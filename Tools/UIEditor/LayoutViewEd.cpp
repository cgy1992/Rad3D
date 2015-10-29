#include "LayoutViewEd.h"
#include "LayoutEditor.h"
#include "AllLookFeel.h"

LayoutViewEd::LayoutViewEd()
{
	mLayout = new MGUI::Layout(NULL);

	mImageBox_Back = new MGUI::ImageBox(NULL, mLayout);
	mImageBox_Back->SetSkin("BackgroundTile.png");
	mImageBox_Back->SetPickFlag(MGUI::ePickFlag::NONE);
	mImageBox_Back->SetAlign(MGUI::eAlign::STRETCH);

	mRectEd = new RectEd(mLayout);
	mRectEd->SetOrder(1000);

	mRectEd->evRectChanged += new cListener1<LayoutViewEd, MGUI::Rect>(this, &LayoutViewEd::OnRectEdChanged_);
	LayoutEditor::Instance()->E_RectChanged += new cListener1<LayoutViewEd, MGUI::Rect>(this, &LayoutViewEd::OnEditRectChanged_);
}

LayoutViewEd::~LayoutViewEd()
{
	delete mLayout;
}

void LayoutViewEd::Layout()
{
	MGUI::Rect rect;

	rect.x = 256;
	rect.y = 32;
	rect.w = MGUI::Engine::Instance()->GetRect().w - 256 - 256;
	rect.h = MGUI::Engine::Instance()->GetRect().h - 32 - 32;

	float skinW = rect.w;
	float skinH = rect.h;
	float backW = (float)mImageBox_Back->GetSkin()->GetWidth();
	float backH = (float)mImageBox_Back->GetSkin()->GetHeight();

	mImageBox_Back->SetUVRect(MGUI::Rect(0, 0, skinW / (float)backW, skinH / (float)backH));

	mLayout->SetRect(rect);
}

void LayoutViewEd::OnRectEdChanged_(MGUI::Rect rect)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();
	MGUI::Layout * layout = LayoutEditor::Instance()->GetEditLayout();

	if (widget == NULL)
		return ;

	while (widget && widget != layout)
	{
		MGUI::Rect ptRect = widget->GetParent()->GetRect();
		MGUI::ClientRect clRect = widget->GetParent()->GetClientRect();

		rect.x -= ptRect.x + clRect.dx0;
		rect.y -= ptRect.y + clRect.dy0;

		widget = widget->GetParent();
	}

	LayoutEditor::Instance()->notifyRectChanged(rect);
}

void LayoutViewEd::OnEditRectChanged_(MGUI::Rect rect)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();
	MGUI::Layout * layout = LayoutEditor::Instance()->GetEditLayout();

	if (widget == NULL)
		return ;

	while (widget && widget != layout)
	{
		MGUI::Rect ptRect = widget->GetParent()->GetRect();
		MGUI::ClientRect clRect = widget->GetParent()->GetClientRect();

		rect.x += ptRect.x + clRect.dx0;
		rect.y += ptRect.y + clRect.dy0;

		widget = widget->GetParent();
	}

	mRectEd->SetEditRect(rect);
}
