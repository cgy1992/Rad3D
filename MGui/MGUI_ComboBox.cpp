#include "MGUI_ComboBox.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(ComboBox, Widget);

	ComboBox::ComboBox(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mDropHeight(128)
		, mPopuped(false)
		, mDropHeightAuto(true)
		, mDropDown(true)
	{
		mEditBox = new EditBox(_lookfeel->GetChild("EditBox"), this);
		mBnDrop = new Button(_lookfeel->GetChild("DropButton"), mEditBox);

		mEditBox->SetAlign(eAlign::STRETCH);
		mEditBox->SetStatic(true);

		mBnDrop->SetAlign(eAlign::RIGHT | eAlign::V_CENTER);

		mListBox = new ListBox(_lookfeel->GetChild("ListBox"), Engine::Instance()->GetTopLayout());
		mListBox->SetVisible(false);

		mEditBox->E_MouseClick += new cListener1<ComboBox, const MouseEvent *>(this, &ComboBox::OnDrop);
		mBnDrop->E_MouseClick += new cListener1<ComboBox, const MouseEvent *>(this, &ComboBox::OnDrop);
		mListBox->E_SelectChanged += new cListener2<ComboBox, const Event *, int>(this, &ComboBox::OnSelectChanged);
		
		InputManager::Instance()->E_MouseDown += mListenerInputMouseDown(this, &ComboBox::OnInputMouseDown);
	}

	ComboBox::~ComboBox()
	{
		delete mListBox;
	}

	void ComboBox::Append(const UString & _text, void * _userData)
	{
		mListBox->Append(_text, _userData);
	}

	void ComboBox::Insert(int _index, const UString & _text, void * _userData)
	{
		mListBox->Insert(_index, _text, _userData);
	}

	void ComboBox::Remove(int _index)
	{
		mListBox->Remove(_index);
	}

	void ComboBox::Clear()
	{
		mListBox->Clear();
	}

	int ComboBox::GetCount()
	{
		return mListBox->GetCount();
	}

	void ComboBox::SetSelectIndex(int _index, bool _sendEvent)
	{
		mListBox->SetSelectIndex(_index, _sendEvent);
	}

	int ComboBox::GetSelectIndex() const
	{
		return mListBox->GetSelectIndex();
	}

	void ComboBox::SetText(int _index, const UString & _text)
	{
		mListBox->SetText(_index, _text);
	}

	const UString & ComboBox::GetText(int _index)
	{
		return mListBox->GetText(_index);
	}

	void * ComboBox::GetUserData(int _index)
	{
		return mListBox->GetUserData(_index);
	}

	void ComboBox::SetItemHeight(float _height)
	{
		mListBox->SetItemHeight(_height);
	}

	float ComboBox::GetItemHeight() const
	{
		return mListBox->GetItemHeight();
	}

	void ComboBox::SetDropHeightAuto(bool _auto)
	{
		mDropHeightAuto = _auto;
	}

	bool ComboBox::GetDropHeightAuto() const
	{
		return mDropHeightAuto;
	}

	void ComboBox::SetDropHeight(float _height)
	{
		mDropHeight = _height;
	}

	float ComboBox::GetDropHeight() const
	{
		return mDropHeight;
	}

	void ComboBox::SetDropDown(bool _down)
	{
		mDropDown = _down;
	}

	bool ComboBox::GetDropDown()
	{
		return mDropDown;
	}

	void ComboBox::SetFontName(const FixedString32 & name)
	{
		mListBox->SetFontName(name);
	}

	FixedString32 ComboBox::GetFontName()
	{
		return mListBox->GetFontName();
	}

	void ComboBox::OnUpdate(float elapsedTime)
	{
		mListBox->SetOrder(GetOrder() + eOrder::TEXT_CURSOR + 1);

		if (mPopuped)
		{
			Rect myRect;

			myRect.x = mAbsRect.x;
			myRect.y = mAbsRect.y + mAbsRect.h;
			myRect.w = mAbsRect.w;

			if (mDropHeightAuto)
			{
				myRect.h = mListBox->GetItemHeight() * mListBox->GetCount() + 1;
				myRect.h += mListBox->GetClientRect().dy0 + mListBox->GetClientRect().dy1;
			}
			else
			{
				myRect.h = mDropHeight;
			}

			if (!mDropDown)
			{
				myRect.y -= myRect.h + mAbsRect.h;
			}

			mListBox->SetRect(myRect);
			mListBox->SetVisible(true);
		}
		else
		{
			mListBox->SetVisible(false);
		}
	}

	void ComboBox::SerializeXML(xml_node root, bool is_out)
	{
		Widget::SerializeXML(root, is_out);

		if (is_out)
		{
			// ItemHeight
			root.append_node("ItemHeight").append_attribute("value", i2str((int)GetItemHeight()).c_str());

			// Font
			Font * font = _getFont();
			if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
			{
				root.append_node("Font").append_attribute("value", GetFontName().c_str());
			}

			// DropHeightAuto
			if (!mDropHeightAuto)
			{
				root.append_node("DropHeightAuto").append_attribute("value", "false");
			}
		}
		else
		{
			// ItemHeight
			xml_node node = root.first_node("ItemHeight");
			if (node != NULL)
			{
				String str = node.first_attribute("value");

				SetItemHeight(str.ToFloat());
			}

			// Font
			node = root.first_node("Font");

			if (node != NULL)
			{
				FixedString32 fontName = node.first_attribute("value");

				SetFontName(fontName);
			}

			// DropHeightAuto
			node = root.first_node("DropHeightAuto");
			if (node != NULL)
			{
				String str = node.first_attribute("value");

				SetDropHeightAuto(str == "true" || str == "TRUE");
			}

			// DropHeight
			node = root.first_node("DropHeight");
			if (node != NULL)
			{
				String str = node.first_attribute("value");

				SetDropHeight(str.ToFloat());
			}
		}
	}
	
	void ComboBox::OnDrop(const MouseEvent * e)
	{
		_drop(!mPopuped);
	}

	void ComboBox::_drop(bool popuped)
	{
		mPopuped = popuped;
	}


	void ComboBox::OnSelectChanged(const Event * e, int index)
	{
		UString text;

		if (index != -1)
			text = GetText(index);

		mEditBox->SetCaption(text);

		Event e2(&E_SelectChanged);
		e2.sender = this;
		E_SelectChanged(&e2, index);

		_drop(false);
	}

	void ComboBox::OnInputMouseDown(const MouseEvent * e)
	{
		if (mListBox->IsVisible() && this->Pick(e->x, e->y) == NULL && mListBox->Pick(e->x, e->y) == NULL)
		{
			_drop(false);
		}
	}

}}