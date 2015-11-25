#include "RenderViewer.h"
#include "CameraController.h"
#include "AllLookFeel.h"
#include "DragDrop.h"
#include "App.h"

ImplementSingleton(RenderViewer);

RenderViewer::RenderViewer(int x, int y, int resolution)
{
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	mLayout = new MGUI::Layout(NULL, NULL);
	mCenterLayout = new MGUI::Layout(NULL, mLayout);
	mCenterLayout->SetAlign(MGUI::eAlign::CENTER);
	mCenterLayout->SetRect(0, 0, (float)resolution * x, (float)resolution * y);

	mImageBox.Resize(x * y);
	mTextBox.Resize(x * y);
	for (int j = 0; j < y; ++j)
	{
		for (int i = 0; i < x; ++i)
		{
			MGUI::Widget * widget = new MGUI::Widget(AllLookFeel::Instance()->GetPanel(), mCenterLayout);
			widget->SetRect((float)i * resolution, (float)j * resolution, (float)resolution, (float)resolution);

			int index = j * x + i;

			mImageBox[index] = new MGUI::ImageBox(NULL, widget);
			mImageBox[index]->SetAlign(MGUI::eAlign::CENTER);
			mImageBox[index]->SetUserData((void *)index);
			mImageBox[index]->E_MouseDown += new cListener1<RenderViewer, const MGUI::MouseEvent *>(this, &RenderViewer::OnResourceDrag);
			mImageBox[index]->E_MouseUp += new cListener1<RenderViewer, const MGUI::MouseEvent *>(this, &RenderViewer::OnResourceOpen);

			mTextBox[index] = new MGUI::TextBox(NULL, widget);
			mTextBox[index]->SetAlign(MGUI::eAlign::H_CENTER | MGUI::eAlign::BOTTOM);
		}
	}

	World::Instance()->MainRenderContext()->SetVisibleCuller(NULL);
	World::Instance()->MainRenderContext()->SetRenderPipeline(NULL);

	mNode.Resize(x * y);
	mObject.Resize(x * y);
	mRenderContext.Resize(x * y);
	for (int i = 0; i < x * y; ++i)
	{
		mNode[i] = new Node;
		mObject[i] = NULL;

		Camera * pCamera = new Camera();
		mCamera.PushBack(pCamera);

		RenderPipelineNodeArray * pRenderPipeline = new RenderPipelineNodeArray(true);
		pRenderPipeline->NodeArray.PushBack(mNode[i]);

		RenderTargetPtr pRenderTarget = HWBufferManager::Instance()->NewRenderTarget(resolution, resolution, ePixelFormat::R8G8B8A8);
		DepthBufferPtr pDepthBuffer = HWBufferManager::Instance()->NewDepthBuffer(resolution, resolution, ePixelFormat::D24);

		mRenderContext[i] = World::Instance()->NewRenderContext(0, "RenderViewer");
		mRenderContext[i]->SetColorClear(eClearMode::ALL, Float4(0, 0, 0, 0));
		mRenderContext[i]->SetRenderTarget(0, pRenderTarget);
		mRenderContext[i]->SetDepthBuffer(pDepthBuffer);
		mRenderContext[i]->SetCamera(pCamera);
		mRenderContext[i]->SetViewport(Viewport(0, 0, resolution, resolution));
		mRenderContext[i]->SetVisibleCuller(NULL);
		mRenderContext[i]->SetShaderProvider(new ShaderProviderStandard);
		mRenderContext[i]->SetRenderPipeline(pRenderPipeline);

		mImageBox[i]->SetSkinAlignedEx(pRenderTarget->GetTexture());
	}

	World::Instance()->MainLight()->SetDirection(Float3(0, -1, 1));

	mCurrentGroupIndex = 0;

	mOpenInFloderDlg = new OpenInFloderDlg;
}

RenderViewer::~RenderViewer()
{
	delete mOpenInFloderDlg;

	for (int i = 0; i < mObject.Size(); ++i)
	{
		safe_delete (mObject[i]);
	}

	for (int i = 0; i < mCamera.Size(); ++i)
	{
		safe_delete (mCamera[i]);
	}

	for (int i = 0; i < mNode.Size(); ++i)
	{
		safe_delete (mNode[i]);
	}

	delete mLayout;
}

void RenderViewer::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float x = 256, y = 32, w = rect.w - x, h = rect.h - y;

	mLayout->SetRect(x, y, w, h);
}

void RenderViewer::SetFloader(const String & floder, const String & key)
{
	mCurrentFloder = floder;

	mItemGroupArray.Clear();
	mCurrentGroupIndex = 0;

	FileSystem fs;
	if (fs.Open(mCurrentFloder))
	{
		ItemGroup group;

		while (fs.NextFile())
		{
			if (fs.FileIsDir())
				continue;

			String filename = fs.FileName();
			String ext = FileHelper::GetExternName(filename);
			ext.ToLower();

			if (mCurrentFloder != "")
			{
				filename = mCurrentFloder + "/" + filename;
			}

			String baseName = FileHelper::GetBaseName(filename);
			if (key != "" && baseName.Find(key, 0) == -1)
			{
				continue;
			}

			if (ext == "mesh")
			{
				group.PushBack(filename);
			}
			else if (ext == "particle")
			{
				group.PushBack(filename);
			}

			if (group.Size() == mObject.Size())
			{
				mItemGroupArray.PushBack(group);
				group.Clear();
			}
		}

		if (group.Size() > 0)
		{
			mItemGroupArray.PushBack(group);
		}
	}

	OnDispaly();
}

void RenderViewer::OnDispaly()
{
	for (int i = 0; i < mObject.Size(); ++i)
	{
		safe_delete (mObject[i]);

		mTextBox[i]->SetCaption(WSTR_(""));
	}

	if (mCurrentGroupIndex >= mItemGroupArray.Size())
		return ;

	const ItemGroup & gp = mItemGroupArray[mCurrentGroupIndex];

	for (int i = 0; i < gp.Size(); ++i)
	{
		String filename = gp[i];
		filename = mCurrentFloder + "/" + filename;
		filename = ResourceManager::Instance()->GetRelativeSourceName(filename);
		if (filename == "")
			continue;

		String ext = FileHelper::GetExternName(filename);
		ext.ToLower();

		if (ext == "mesh")
		{
			MeshSourcePtr source = MeshManager::Instance()->LoadMesh(filename);
			
			Mesh * mesh = new Mesh;
			mesh->SetSource(source);

			mObject[i] = mesh;
		}
		else if (ext == "particle")
		{
			PS_SourcePtr source = PS_Manager::Instance()->LoadSource(filename);
			
			ParticleSystem * ps = new ParticleSystem;
			ps->SetSource(source);

			mObject[i] = ps;
		}

		if (mObject[i] == NULL)
			continue;

		mNode[i]->Attach(mObject[i]);

		OnUpdateCamera(mRenderContext[i]->GetCamera(), mObject[i]);

		mTextBox[i]->SetCaption(FileHelper::GetBaseName(filename).c_wstr());
	}
}

void RenderViewer::OnUpdateCamera(Camera * cam, Node * node)
{
	Aabb bound = node->GetWorldAabb();

	float size = 0;
	size = Max(size, bound.GetSize().x);
	size = Max(size, bound.GetSize().y);
	size = Max(size, bound.GetSize().z);

	if (size == 0)
		size = 100.0f;

	CameraController cc(cam);
	cc.SetLookAt(bound.GetCenter());
	cc.SetScroll(size * 2.0f);
	cc.SetYaw(0);
	cc.SetPitch(45);
	cc.ForceUpdate();
}

bool RenderViewer::CanPrev()
{
	return mCurrentGroupIndex > 0;
}

bool RenderViewer::CanNext()
{
	return mCurrentGroupIndex < mItemGroupArray.Size() - 1;
}

void RenderViewer::OnPrevGroup()
{
	if (mCurrentGroupIndex > 0)
	{
		--mCurrentGroupIndex;

		OnDispaly();
	}
}

void RenderViewer::OnNextGroup()
{
	if (mCurrentGroupIndex < mItemGroupArray.Size() - 1)
	{
		++mCurrentGroupIndex;

		OnDispaly();
	}
}

void RenderViewer::OnSearch(const String & key)
{
	if (key != "")
	{
		SetFloader(mCurrentFloder, key);
	}
}

void RenderViewer::OnResourceDrag(const MGUI::MouseEvent * e)
{
	if (mCurrentGroupIndex >= mItemGroupArray.Size() ||
		e->id != InputCode::MKC_LEFT)
		return ;

	const ItemGroup & gp = mItemGroupArray[mCurrentGroupIndex];
	int index = (int)e->sender->GetUserData();

	if (index < gp.Size())
	{
		String filename = gp[index];

		Util_SetDrapFile(filename);
	}
}

void RenderViewer::OnResourceOpen(const MGUI::MouseEvent * e)
{
	if (mCurrentGroupIndex >= mItemGroupArray.Size() ||
		e->id != InputCode::MKC_RIGHT)
		return ;

	const ItemGroup & gp = mItemGroupArray[mCurrentGroupIndex];
	int index = (int)e->sender->GetUserData();

	if (index < gp.Size())
	{
		String filename = gp[index];
		filename.Replace('/', '\\');

		mOpenInFloderDlg->DoModal(e->x, e->y, filename);
	}
}
