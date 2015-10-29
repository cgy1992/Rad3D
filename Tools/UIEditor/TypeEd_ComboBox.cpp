#include "TypeEd_ComboBox.h"
#include "AllLookFeel.h"

TypeEd_ComboBox::TypeEd_ComboBox(MGUI::Widget * editbox, MGUI::Widget * panel)
{
	d_assert (TYPE_OF(MGUI::ComboBox, editbox));

	mComboBox = (MGUI::ComboBox *)editbox;

	float top = 0;
	const float K_Space = 8;

	mLabel_ItemHeight = new MGUI::Label(NULL, panel);
	mLabel_ItemHeight->SetCaption(L"ItemHeight:");
	mLabel_ItemHeight->SetRect(0, top, 64, 24);

	mEditBox_ItemHeight = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), panel);
	mEditBox_ItemHeight->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	mLabel_Font = new MGUI::Label(NULL, panel);
	mLabel_Font->SetCaption(L"Font:");
	mLabel_Font->SetRect(0, top, 64, 24);

	mCombo_Font = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), panel);
	mCombo_Font->SetRect(80, top, 162, 24);

	// Init
	String caption;
	caption.Format("%d", (int)mComboBox->GetItemHeight());
	mEditBox_ItemHeight->SetCaption(caption.c_wstr());

	for (int i = 0; i < MGUI::FontManager::Instance()->GetFontCount(); ++i)
	{
		MGUI::Font * pFont = MGUI::FontManager::Instance()->GetFont(i);
		mCombo_Font->Append(pFont->GetName().c_wstr(), pFont);

		if (mComboBox->_getFont() == pFont)
			mCombo_Font->SetSelectIndex(i);
	}

	mEditBox_ItemHeight->E_KeyLostFocus += new cListener1<TypeEd_ComboBox, const MGUI::FocusEvent *>(this, &TypeEd_ComboBox::OnItemHeightChanged);
	mCombo_Font->E_SelectChanged += new cListener2<TypeEd_ComboBox, const MGUI::Event *, int>(this, &TypeEd_ComboBox::OnFontChanged);
}

TypeEd_ComboBox::~TypeEd_ComboBox()
{
	delete mLabel_ItemHeight;
	delete mLabel_Font;
	delete mEditBox_ItemHeight;
	delete mCombo_Font;
}

void TypeEd_ComboBox::OnItemHeightChanged(const MGUI::FocusEvent * e)
{
	String itemHeight;
	itemHeight.FromUnicode(mEditBox_ItemHeight->GetCaption().c_str());

	mComboBox->SetItemHeight(itemHeight.ToFloat());
}

void TypeEd_ComboBox::OnFontChanged(const MGUI::Event * e, int index)
{
	d_assert (index >= 0);

	MGUI::Font * font = (MGUI::Font *)mCombo_Font->GetUserData(index);

	mComboBox->SetFontName(font->GetName());
}