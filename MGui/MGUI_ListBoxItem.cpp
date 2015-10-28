#include "MGUI_ListBoxItem.h"
#include "MGUI_Engine.h"
#include "MGUI_ListBox.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(ListBoxItem, Widget);

	ListBoxItem::ListBoxItem(ListBox * _listBox, const LookFeel * _lookfeel,
									   const UString & _text, void * _userData)
		: Widget(_lookfeel, _listBox->GetClientWidget())
	{
		mListBox = _listBox;
		mUserData = _userData;

		mTextBox = new TextBox(_lookfeel, this);
		mTextBox->SetAlign(eAlign::LEFT | eAlign::V_CENTER);
		mTextBox->SetPickFlag(ePickFlag::NONE);
		mTextBox->SetInheritState(true);
		mTextBox->SetFont(_listBox->_getFont());

		SetText(_text);
	}

	ListBoxItem::~ListBoxItem()
	{
	}

	void ListBoxItem::OnUpdateWidgetState()
	{
		Widget::OnUpdateWidgetState();

		if (mListBox->IsEnable() && mListBox->GetSelectItem() == this)
			mState = eWidgetState::SELECT;
	}

	void ListBoxItem::OnMouseDown(int _id, float _x, float _y)
	{
		if (_id == InputCode::MKC_LEFT)
		{
			mListBox->SetSelectItem(this);
		}
	}

	void ListBoxItem::OnTouchDown(int _id, float _x, float _y)
	{
		OnMouseDown(InputCode::MKC_LEFT, _x, _y);
	}

}}