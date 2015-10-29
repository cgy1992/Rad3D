#include "TypeEd_Button.h"
#include "AllLookFeel.h"

TypeEd_Button::TypeEd_Button(MGUI::Widget * button, MGUI::Widget * panel)
{
	d_assert (TYPE_OF(MGUI::Button, button));

	mButton = (MGUI::Button *)button;

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

	// Init
	mEditBox_Caption->SetCaption(mButton->GetCaption());
	
	for (int i = 0; i < MGUI::FontManager::Instance()->GetFontCount(); ++i)
	{
		MGUI::Font * pFont = MGUI::FontManager::Instance()->GetFont(i);
		mCombo_Font->Append(pFont->GetName().c_wstr(), pFont);

		if (mButton->_getFont() == pFont)
			mCombo_Font->SetSelectIndex(i);
	}

	mEditBox_Caption->E_KeyLostFocus += new cListener1<TypeEd_Button, const MGUI::FocusEvent *>(this, &TypeEd_Button::OnCaptionChanged);
	mCombo_Font->E_SelectChanged += new cListener2<TypeEd_Button, const MGUI::Event *, int>(this, &TypeEd_Button::OnFontChanged);
}

TypeEd_Button::~TypeEd_Button()
{
	delete mLabel_Caption;
	delete mLabel_Font;
	delete mEditBox_Caption;
	delete mCombo_Font;
}

void TypeEd_Button::OnCaptionChanged(const MGUI::FocusEvent * e)
{
	mButton->SetCaption(mEditBox_Caption->GetCaption());
}

void TypeEd_Button::OnFontChanged(const MGUI::Event * e, int index)
{
	d_assert (index >= 0);

	MGUI::Font * font = (MGUI::Font *)mCombo_Font->GetUserData(index);

	mButton->SetFontName(font->GetName());
}