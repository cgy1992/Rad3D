#include "TypeEd_EditView.h"
#include "AllLookFeel.h"

TypeEd_EditView::TypeEd_EditView(MGUI::Widget * editbox, MGUI::Widget * panel)
{
	d_assert (TYPE_OF(MGUI::EditView, editbox));

	mEditView = (MGUI::EditView *)editbox;

	float top = 0;
	const float K_Space = 8;

	mLabel_Font = new MGUI::Label(NULL, panel);
	mLabel_Font->SetCaption(L"Font:");
	mLabel_Font->SetRect(0, top, 64, 24);

	mCombo_Font = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), panel);
	mCombo_Font->SetRect(80, top, 162, 24);

	// Init
	for (int i = 0; i < MGUI::FontManager::Instance()->GetFontCount(); ++i)
	{
		MGUI::Font * pFont = MGUI::FontManager::Instance()->GetFont(i);
		mCombo_Font->Append(pFont->GetName().c_wstr(), pFont);

		if (mEditView->_getFont() == pFont)
			mCombo_Font->SetSelectIndex(i);
	}

	mCombo_Font->E_SelectChanged += new cListener2<TypeEd_EditView, const MGUI::Event *, int>(this, &TypeEd_EditView::OnFontChanged);
}

TypeEd_EditView::~TypeEd_EditView()
{
	delete mLabel_Font;
	delete mCombo_Font;
}

void TypeEd_EditView::OnFontChanged(const MGUI::Event * e, int index)
{
	d_assert (index >= 0);

	MGUI::Font * font = (MGUI::Font *)mCombo_Font->GetUserData(index);

	mEditView->SetFontName(font->GetName());
}