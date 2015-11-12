#include "MGUI_Engine.h"
#include "MGUI.h"

namespace Rad {

	using namespace MGUI;

	class RootLayout : public Widget
	{
	public:
		RootLayout() : Widget(NULL, NULL)
		{
		}

		virtual ~RootLayout()
		{
		}

		virtual void UpdateRenderItem(Layout * _layout)
		{
			mClipRect = mAbsClient;
		}
	};

	//
	ImplementSingleton(Engine);

	Engine::Engine()
	{
		REG_WIDGET<Widget>();
		REG_WIDGET<Button>();
		REG_WIDGET<CheckBox>();
		REG_WIDGET<ComboBox>();
		REG_WIDGET<EditBox>();
		REG_WIDGET<EditView>();
		REG_WIDGET<ImageBox>();
		REG_WIDGET<Label>();
		REG_WIDGET<ListBox>();
		REG_WIDGET<Panel>();
		REG_WIDGET<ProgressBarH>();
		REG_WIDGET<ProgressBarV>();
		REG_WIDGET<ScrollBarH>();
		REG_WIDGET<ScrollBarV>();
		REG_WIDGET<ScrollViewH>();
		REG_WIDGET<ScrollViewV>();
		REG_WIDGET<SliderBarH>();
		REG_WIDGET<SliderBarV>();

		mLocale = new Locale;
		mLookFeelManager = new LookFeelManager;
		mClipboard = new Clipboard;
		mFontManager = new FontManager;
		mInputManager = new InputManager;
		mHelper = new Helper;

		mShaderFX = ShaderFXManager::Instance()->Load("MGUI", "Shader/MGUI.mfx");
		d_assert (mShaderFX != NULL && mShaderFX->GetPassCount() > 0);

		mRootLayout = new RootLayout;
		mTopLayout = new Layout(NULL, mRootLayout);
		mTopLayout->SetOrder(eOrder::TOP);
		mTopLayout->SetPickFlag(ePickFlag::CHILD);
		mTopLayout->SetAlign(eAlign::STRETCH);

		OnResize(RenderSystem::Instance()->GetConfig().width, RenderSystem::Instance()->GetConfig().height);
	}

	Engine::~Engine()
	{
		delete mRootLayout;
		delete mHelper;
		delete mInputManager;
		delete mFontManager;
		delete mClipboard;
		delete mLookFeelManager;
		delete mLocale;
	}

	void Engine::OnResize(int w, int h)
	{
		mRootLayout->SetRect(0, 0, (float)w, (float)h);

		mInvSize.x = 1.0f / w;
		mInvSize.y = 1.0f / h;

		E_Resize(w, h);
	}

	void Engine::Update(float elapsedTime)
	{
		profile_code();

		mRootLayout->Update(elapsedTime);
	}

	void Engine::Render()
	{
		profile_code();

		Viewport vp;
		vp.w = (int)mRootLayout->GetRect().w;
		vp.h = (int)mRootLayout->GetRect().h;
		RenderSystem::Instance()->SetViewport(vp);

		Float4 tm;
		tm.x = mInvSize.x * 2;
		tm.y = mInvSize.y * 2;
		tm.z = 0;
		tm.w = 1;

		mShaderFX->GetPass(0)->SetConst("u_Transform", tm);

		RenderSystem::Instance()->SetRenderState(eFillMode::SOLID, eCullMode::NONE, eDepthMode::NONE, eBlendMode::ALPHA_BLEND, eColorMode::ALL);
		RenderSystem::Instance()->SetShaderPass(mShaderFX->GetPass(0), false);

		if (!mRootLayout->IsVisible())
			return ;

		mRootLayout->UpdateRenderItem(NULL);

		for (int i = 0; i < mRootLayout->GetChildCount(); ++i)
		{
			d_assert (KIND_OF(Layout, mRootLayout->GetChild(i)));

			Layout * pLayout = (Layout *)mRootLayout->GetChild(i);

			pLayout->UpdateRenderItem();
		}

		for (int i = 0; i < mRootLayout->GetChildCount(); ++i)
		{
			d_assert (KIND_OF(Layout, mRootLayout->GetChild(i)));

			Layout * pLayout = (Layout *)mRootLayout->GetChild(i);

			pLayout->DoRender();
		}
	}

}