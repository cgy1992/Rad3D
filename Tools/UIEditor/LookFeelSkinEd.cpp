#include "LookFeelSkinEd.h"
#include "LookFeelEditor.h"


LookFeelSkinEd::LookFeelSkinEd()
	: mEnableDrag(false)
{
	mLayout = new MGUI::Layout(NULL);

	mImageBox_Back = new MGUI::ImageBox(NULL, mLayout);
	mImageBox_Back->SetSkin("BackgroundTile.png");
	mImageBox_Back->SetPickFlag(MGUI::ePickFlag::NONE);

	mImageBox_Skin = new MGUI::ImageBox(NULL, mLayout);
	mImageBox_Skin->SetOrder(1);

	mRectEd = new RectEd(mImageBox_Skin);

	LookFeelEditor::Instance()->E_EditSkinChanged += new cListener1<LookFeelSkinEd, Texture *>(this, &LookFeelSkinEd::OnEditSkinChanged_);
	LookFeelEditor::Instance()->E_EditRectChanged += new cListener1<LookFeelSkinEd, MGUI::Rect>(this, &LookFeelSkinEd::OnEditRectChanged_);
	LookFeelEditor::Instance()->E_EditClientChanged += new cListener1<LookFeelSkinEd, MGUI::ClientRect>(this, &LookFeelSkinEd::OnEditClientChanged_);

	mRectEd->evRectChanged += new cListener1<LookFeelSkinEd, MGUI::Rect>(this, &LookFeelSkinEd::OnRectEdChanged_);

	mImageBox_Skin->E_MouseDown += new cListener1<LookFeelSkinEd, const MGUI::MouseEvent *>(this, &LookFeelSkinEd::OnSkinMouseDown_);
	mImageBox_Skin->E_MouseUp += new cListener1<LookFeelSkinEd, const MGUI::MouseEvent *>(this, &LookFeelSkinEd::OnSkinMouseUp_);
	mImageBox_Skin->E_MouseMove += new cListener1<LookFeelSkinEd, const MGUI::MouseEvent *>(this, &LookFeelSkinEd::OnSkinMouseMove_);
}

LookFeelSkinEd::~LookFeelSkinEd()
{
	delete mLayout;
}

void LookFeelSkinEd::Layout()
{
	MGUI::Rect rect;

	rect.x = 256;
	rect.y = 32;
	rect.w = MGUI::Engine::Instance()->GetRect().w - 256 - 256;
	rect.h = MGUI::Engine::Instance()->GetRect().h - 32 - 32;

	mLayout->SetRect(rect);
}

void LookFeelSkinEd::OnEditSkinChanged_(Texture * _skin)
{
	mImageBox_Skin->SetSkinEx(_skin);

	if (_skin)
	{
		float skinW = (float)_skin->GetWidth();
		float skinH = (float)_skin->GetHeight();
		float backW = (float)mImageBox_Back->GetSkin()->GetWidth();
		float backH = (float)mImageBox_Back->GetSkin()->GetHeight();

		mImageBox_Back->SetRect(0, 0, skinW, skinH);
		mImageBox_Skin->SetRect(0, 0, skinW, skinH);

		mImageBox_Back->SetUVRect(MGUI::Rect(0, 0, skinW / (float)backW, skinH / (float)backH));
	}
	else
	{
		mImageBox_Back->SetRect(0, 0, 0, 0);
		mImageBox_Skin->SetRect(0, 0, 0, 0);
	}
}

void LookFeelSkinEd::OnEditRectChanged_(MGUI::Rect _rect)
{
	mRectEd->SetEditRect(_rect);
}

void LookFeelSkinEd::OnEditClientChanged_(MGUI::ClientRect _rect)
{
	mRectEd->SetEditClient(_rect);
}

void LookFeelSkinEd::OnRectEdChanged_(MGUI::Rect _rect)
{
	LookFeelEditor::Instance()->_notifyRectChanged(_rect);
}

void LookFeelSkinEd::OnSkinMouseDown_(const MGUI::MouseEvent * e)
{
	if (e->id == InputCode::MKC_RIGHT)
	{
		mTmpRect = mImageBox_Skin->GetRect();
		mTmpX = e->x;
		mTmpY = e->y;
		mEnableDrag = true;

		MGUI::InputManager::Instance()->SetForceFocus(true);
	}
}

void LookFeelSkinEd::OnSkinMouseUp_(const MGUI::MouseEvent * e)
{
	if (e->id == InputCode::MKC_RIGHT)
	{
		mEnableDrag = false;
		MGUI::InputManager::Instance()->SetForceFocus(false);
	}
}

void LookFeelSkinEd::OnSkinMouseMove_(const MGUI::MouseEvent * e)
{
	if (!mEnableDrag)
		return ;

	float dx = e->x - mTmpX;
	float dy = e->y - mTmpY;

	MGUI::Rect rc = mTmpRect;

	rc.x += dx, rc.y += dy;

	float rdx = rc.w;
	float rdy = rc.h;

	if (rc.x > 0)
	{
		rc.x = 0;
		rc.w = rdx;
	}

	if (rc.y > 0)
	{
		rc.y = 0;
		rc.h = rdy;
	}
		
	mImageBox_Back->SetRect(rc);
	mImageBox_Skin->SetRect(rc);
}

