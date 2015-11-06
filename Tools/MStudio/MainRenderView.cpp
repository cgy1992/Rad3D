#include "MainRenderView.h"
#include "MainFrame.h"
#include "Editor.h"

ImplementSingleton(MainRenderView);

MainRenderView::MainRenderView()
{
	mLayout = new MGUI::Layout(NULL, NULL);
	mLayout->SetOrder(-1);
	mLayout->E_MouseDoubleClick += new cListener1<MainRenderView, const MGUI::MouseEvent *>(this, &MainRenderView::OnDoubleClick);

	mLayout->E_MouseDown += new cListener1<MainRenderView, const MGUI::MouseEvent *>(this, &MainRenderView::OnMouseDown);
	mLayout->E_MouseUp += new cListener1<MainRenderView, const MGUI::MouseEvent *>(this, &MainRenderView::OnMouseUp);
	mLayout->E_MouseDrag += new cListener1<MainRenderView, const MGUI::MouseEvent *>(this, &MainRenderView::OnMouseDrag);

	Editor::Instance()->E_Update += new cListener0<MainRenderView>(this, &MainRenderView::OnUpdate);

	mRangePick = false;

	mRectWidget = new MGUI::Widget(NULL, mLayout);
	mRectWidget->SetPickFlag(MGUI::ePickFlag::NONE);
	mRectWidget->SetVisible(false);
	mRectWidget->SetOrder(1);

	mLeft = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mRectWidget);
	mTop = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mRectWidget);
	mRight = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mRectWidget);
	mBottom = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mRectWidget);

	mLeft->SetPickFlag(MGUI::ePickFlag::NONE);
	mTop->SetPickFlag(MGUI::ePickFlag::NONE);
	mRight->SetPickFlag(MGUI::ePickFlag::NONE);
	mBottom->SetPickFlag(MGUI::ePickFlag::NONE);

	mLeft->SetColor(Float4(0, 1, 0, 0.8f));
	mTop->SetColor(Float4(0, 1, 0, 0.8f));
	mRight->SetColor(Float4(0, 1, 0, 0.8f));
	mBottom->SetColor(Float4(0, 1, 0, 0.8f));

	mLeft->SetRect(0, 0, 1, 1);
	mTop->SetRect(0, 0, 1, 1);
	mRight->SetRect(0, 0, 1, 1);
	mBottom->SetRect(0, 0, 1, 1);

	mLeft->SetAlign(MGUI::eAlign::LEFT | MGUI::eAlign::V_STRETCH);
	mTop->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mRight->SetAlign(MGUI::eAlign::RIGHT | MGUI::eAlign::V_STRETCH);
	mBottom->SetAlign(MGUI::eAlign::BOTTOM | MGUI::eAlign::H_STRETCH);
}

MainRenderView::~MainRenderView()
{
	delete mLayout;
}

void MainRenderView::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	mLayout->SetRect(rect);
}

bool MainRenderView::IsMouseFoucsed()
{
	Float2 pt = IMouse::Instance()->GetPosition();

	return MGUI::Engine::Instance()->GetWidget(pt.x, pt.y) == mLayout &&
		MGUI::InputManager::Instance()->GetMouseFocusedWidget() == mLayout;
}

void MainRenderView::OnDoubleClick(const MGUI::MouseEvent * e)
{
	if (Editor::Instance()->GetFlags() & FLAG_PICK_ABLE)
	{
		Editor::Instance()->OnPickNode();
	}
}

void MainRenderView::OnUpdate()
{
	if (Editor::Instance()->GetFlags() & FLAG_PICK_ABLE)
	{
		if (IKeyboard::Instance()->KeyPressed(InputCode::KC_LSHIFT))
		{
			mRangePick = true;
		}
		else
		{
			mRangePick = false;
		}
	}
	else
	{
		mRangePick = false;
	}

	if (!mRangePick && mRectWidget->IsVisible())
	{
		mRectWidget->SetVisible(false);
	}

	if (mRectWidget->IsVisible())
	{
		float x1, y1, x2, y2;

		x1 = Min(mMousePos1.x, mMousePos2.x);
		y1 = Min(mMousePos1.y, mMousePos2.y);
		x2 = Max(mMousePos1.x, mMousePos2.x);
		y2 = Max(mMousePos1.y, mMousePos2.y);

		MGUI::Rect rect;
		rect.x = x1;
		rect.y = y1;
		rect.w = x2 - x1;
		rect.h = y2 - y1;

		mRectWidget->SetRect(rect);
	}
}

void MainRenderView::OnMouseDown(const MGUI::MouseEvent * e)
{
	if (mRangePick)
	{
		Gizmo::Instance()->SetEnable(false);

		mMousePos1.x = e->x;
		mMousePos1.y = e->y;

		mMousePos2 = mMousePos1;

		mRectWidget->SetVisible(true);
	}
}

void MainRenderView::OnMouseUp(const MGUI::MouseEvent * e)
{
	if (mRangePick)
	{
		mMousePos2.x = e->x;
		mMousePos2.y = e->y;

		float x1, y1, x2, y2;

		x1 = Min(mMousePos1.x, mMousePos2.x);
		y1 = Min(mMousePos1.y, mMousePos2.y);
		x2 = Max(mMousePos1.x, mMousePos2.x);
		y2 = Max(mMousePos1.y, mMousePos2.y);

		MGUI::Rect rect;
		rect.x = x1;
		rect.y = y1;
		rect.w = x2 - x1;
		rect.h = y2 - y1;

		if (rect.w > 0 && rect.h > 0)
		{
			Editor::Instance()->OnPickNodeRect(rect);
		}

		mRangePick = false;

		mRectWidget->SetVisible(false);
	}

	Gizmo::Instance()->SetEnable(true);
}

void MainRenderView::OnMouseDrag(const MGUI::MouseEvent * e)
{
	if (mRangePick && mRectWidget->IsVisible())
	{
		mMousePos2.x = e->x;
		mMousePos2.y = e->y;
	}
}