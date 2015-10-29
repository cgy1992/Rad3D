#include "TypeEd_Label.h"
#include "AllLookFeel.h"
#include "App.h"

TypeEd_Label::TypeEd_Label(MGUI::Widget * label, MGUI::Widget * panel)
{
	d_assert (TYPE_OF(MGUI::Label, label));

	mLabel = (MGUI::Label *)label;

	float top = 0;
	const float K_Space = 8;

	mLabel_Caption = new MGUI::Label(NULL, panel);
	mLabel_Caption->SetCaption(L"Caption:");
	mLabel_Caption->SetRect(0, top, 64, 24);

	mEditBox_Caption = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), panel);
	mEditBox_Caption->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	mLabel_Font = new MGUI::Label(NULL, panel);
	mLabel_Font->SetCaption(L"Font:");
	mLabel_Font->SetRect(0, top, 64, 24);

	mCombo_Font = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), panel);
	mCombo_Font->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	mLabel_TextAlign = new MGUI::Label(NULL, panel);
	mLabel_TextAlign->SetCaption(L"TAlign:");
	mLabel_TextAlign->SetRect(0, top, 64, 24);

	mCombo_TextAlign = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), panel);
	mCombo_TextAlign->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	mLabel_TextColor = new MGUI::Label(NULL, panel);
	mLabel_TextColor->SetCaption(L"TColor:");
	mLabel_TextColor->SetRect(0, top, 64, 24);
	
	mWidget_TextColor = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), panel);
	mWidget_TextColor->SetRect(80, top, 162, 24);

	//
	mEditBox_Caption->SetCaption(mLabel->GetCaption());
	
	for (int i = 0; i < MGUI::FontManager::Instance()->GetFontCount(); ++i)
	{
		MGUI::Font * pFont = MGUI::FontManager::Instance()->GetFont(i);
		mCombo_Font->Append(pFont->GetName().c_wstr(), pFont);

		if (mLabel->_getFont() == pFont)
			mCombo_Font->SetSelectIndex(i);
	}

	mCombo_TextAlign->Append(L"Left");
	mCombo_TextAlign->Append(L"Center");
	mCombo_TextAlign->Append(L"Right");
	mCombo_TextAlign->SetSelectIndex(0);

	MGUI::eAlign align = mLabel->GetTextAlign();
	if (align == MGUI::eAlign::LEFT)
		mCombo_TextAlign->SetSelectIndex(0);
	else if (align == MGUI::eAlign::H_CENTER)
		mCombo_TextAlign->SetSelectIndex(1);
	else if (align == MGUI::eAlign::RIGHT)
		mCombo_TextAlign->SetSelectIndex(2);

	mWidget_TextColor->SetColor(mLabel->GetTextColor());

	mEditBox_Caption->E_KeyLostFocus += new cListener1<TypeEd_Label, const MGUI::FocusEvent *>(this, &TypeEd_Label::OnCaptionChanged);
	mCombo_Font->E_SelectChanged += new cListener2<TypeEd_Label, const MGUI::Event *, int>(this, &TypeEd_Label::OnFontChanged);
	mCombo_TextAlign->E_SelectChanged += new cListener2<TypeEd_Label, const MGUI::Event *, int>(this, &TypeEd_Label::OnTextAlignChanged);
	mWidget_TextColor->E_MouseClick += new cListener1<TypeEd_Label, const MGUI::MouseEvent *>(this, &TypeEd_Label::OnTextColorChanged);
}

TypeEd_Label::~TypeEd_Label()
{
	delete mLabel_Caption;
	delete mEditBox_Caption;
	delete mLabel_Font;
	delete mCombo_Font;
	delete mLabel_TextAlign;
	delete mCombo_TextAlign;
	delete mLabel_TextColor;
	delete mWidget_TextColor;
}

void TypeEd_Label::OnCaptionChanged(const MGUI::FocusEvent * e)
{
	mLabel->SetCaption(mEditBox_Caption->GetCaption());
}

void TypeEd_Label::OnFontChanged(const MGUI::Event * e, int index)
{
	d_assert (index >= 0);

	MGUI::Font * font = (MGUI::Font *)mCombo_Font->GetUserData(index);

	mLabel->SetFontName(font->GetName());
}

void TypeEd_Label::OnTextAlignChanged(const MGUI::Event * e, int index)
{
	d_assert (index >= 0);

	MGUI::eAlign align = MGUI::eAlign::LEFT;

	switch (index)
	{
	case 0:
		align = MGUI::eAlign::LEFT;
		break;

	case 1:
		align = MGUI::eAlign::H_CENTER;
		break;

	case 2:
		align = MGUI::eAlign::RIGHT;
		break;
	}

	mLabel->SetTextAlign(align);
}

void TypeEd_Label::OnTextColorChanged(const MGUI::MouseEvent * e)
{
	Float4 color = mLabel->GetTextColor();
	color.a = 1;

	static COLORREF crCustColors[6];
	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = App::Instance()->_hWnd();
	cc.hInstance = NULL;
	cc.rgbResult = RGB(color.r * 255, color.g * 255, color.b * 255);
	cc.lpCustColors = crCustColors;
	cc.Flags = CC_RGBINIT|CC_FULLOPEN;
	cc.lCustData = 0;
	cc.lpfnHook = NULL;
	cc.lpTemplateName = NULL;

	if (ChooseColor(&cc))
	{
		color.r = GetRValue(cc.rgbResult) / 255.0f;
		color.g = GetGValue(cc.rgbResult) / 255.0f;
		color.b = GetBValue(cc.rgbResult) / 255.0f;
		mLabel->SetTextColor(color);
		mWidget_TextColor->SetColor(color);
	}
}
