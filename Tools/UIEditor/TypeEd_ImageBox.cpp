#include "TypeEd_ImageBox.h"
#include "AllLookFeel.h"

TypeEd_ImageBox::TypeEd_ImageBox(MGUI::Widget * widget, MGUI::Widget * panel)
{
	d_assert (TYPE_OF(MGUI::ImageBox, widget));

	mImageBox = (MGUI::ImageBox *)widget;

	float top = 0;
	const float K_Space = 8;

	mLabel_Image = new MGUI::Label(NULL, panel);
	mLabel_Image->SetCaption(L"Image:");
	mLabel_Image->SetRect(0, top, 64, 24);

	mEditBox_Image = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), panel);
	mEditBox_Image->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	mLabel_UVRect = new MGUI::Label(NULL, panel);
	mLabel_UVRect->SetCaption(L"UVRect");
	mLabel_UVRect->SetRect(0, top, 162, 24);

	mEditBox_UVRect = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), panel);
	mEditBox_UVRect->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Init
	mEditBox_Image->SetCaption(mImageBox->GetSkin() != NULL ? mImageBox->GetSkin()->GetName().c_wstr() : L"");

	mEditBox_UVRect->SetCaption(mImageBox->GetUVRect().ToString().c_wstr());

	mEditBox_Image->E_KeyLostFocus += new cListener1<TypeEd_ImageBox, const MGUI::FocusEvent *>(this, &TypeEd_ImageBox::OnImageChanged);
	mEditBox_UVRect->E_KeyLostFocus += new cListener1<TypeEd_ImageBox, const MGUI::FocusEvent *>(this, &TypeEd_ImageBox::OnUVRectChanged);
}

TypeEd_ImageBox::~TypeEd_ImageBox()
{
	delete mLabel_Image;
	delete mEditBox_Image;

	delete mLabel_UVRect;
	delete mEditBox_UVRect;
}

void TypeEd_ImageBox::OnImageChanged(const MGUI::FocusEvent * e)
{
	String imageName;
	imageName.FromUnicode(mEditBox_Image->GetCaption().c_str());

	mImageBox->SetSkin(imageName);
}

void TypeEd_ImageBox::OnUVRectChanged(const MGUI::FocusEvent * e)
{
	String uvRectStr;
	uvRectStr.FromUnicode(mEditBox_UVRect->GetCaption().c_str());

	MGUI::Rect rect;
	rect.FromString(uvRectStr);

	mImageBox->SetRect(rect);
}

