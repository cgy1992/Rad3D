#include "MGUI_ListBox.h"
#include "MGUI_Helper.h"
#include "MGUI_Layout.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(ListBox, Panel);

	ListBox::ListBox(const LookFeel * _lookfeel, Widget * _parent)
		: Panel(_lookfeel, _parent)
		, mSelectIndex(-1)
	{
		mItemHeight = 24;
		mAutoAdjustHeight = false;
		mFont = FontManager::Instance()->GetDefaultFont();

		mItemLookFeel = NULL;
		if (_lookfeel != NULL)
			mItemLookFeel = _lookfeel->GetChild("ListBoxItem");
	}

	ListBox::~ListBox()
	{
	}

	ListBoxItem * ListBox::Append(const UString & _text, void * _userData)
	{
		return Insert(GetCount(), _text, _userData);
	}

	ListBoxItem * ListBox::Insert(int _index, const UString & _text, void * _userData)
	{
		ListBoxItem * pListBoxItem = new ListBoxItem(this, mItemLookFeel, _text, _userData);

		mItems.Insert(_index, pListBoxItem);

		if (mSelectIndex >= _index)
			SetSelectIndex(mSelectIndex + 1);

		return pListBoxItem;
	}

	void ListBox::Remove(int _index)
	{
		delete mItems[_index];
		mItems.Erase(_index);

		if (mSelectIndex >= _index)
			SetSelectIndex(mSelectIndex - 1);
	}

	void ListBox::Clear()
	{
		SetSelectIndex(-1);

		for (int i = 0; i < mItems.Size(); ++i)
			delete mItems[i];
		
		mItems.Clear();
	}

	int ListBox::GetCount()
	{
		return mItems.Size();
	}

	void ListBox::SetText(int _index, const UString & _text)
	{
		mItems[_index]->SetText(_text);
	}

	const UString & ListBox::GetText(int _index)
	{
		return mItems[_index]->GetText();
	}

	void * ListBox::GetUserData(int _index)
	{
		return mItems[_index]->GetUserData();
	}

	ListBoxItem * ListBox::GetItem(int _index)
	{
		return mItems[_index];
	}

	void ListBox::SetItemHeight(float _height)
	{
		mItemHeight = _height;
	}

	float ListBox::GetItemHeight() const
	{
		return mItemHeight;
	}

	void ListBox::SetAutoAdjustHeight(bool _auto)
	{
		mAutoAdjustHeight = _auto;
	}

	bool ListBox::GetAutoAdjustHeight() const
	{
		return mAutoAdjustHeight;
	}

	void ListBox::SetFontName(const FixedString32 & name)
	{
		Font * p = FontManager::Instance()->GetFont(name);
		if (p)
		{
			mFont = p;
		}
	}

	FixedString32 ListBox::GetFontName() const
	{
		Font * font = mFont;
		if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
		{
			return font->GetName();
		}

		return "";
	}

	void ListBox::SetSelectIndex(int _index, bool _sendEvent)
	{
		d_assert (_index < GetCount());

		if (_index != mSelectIndex)
		{
			mSelectIndex = _index;

			if (_sendEvent)
			{
				Event e(&E_SelectChanged);
				e.sender = this;

				E_SelectChanged(&e, mSelectIndex);
			}
		}
	}

	int ListBox::GetSelectIndex() const
	{
		return mSelectIndex;
	}

	void ListBox::SetSelectItem(ListBoxItem * _item)
	{
		if (_item != NULL)
		{
			for (int i = 0; i < GetCount(); ++i)
			{
				ListBoxItem * item = mItems[i];
				if (item == _item)
				{
					SetSelectIndex(i);
					return ;
				}
			}

			d_assert (0);
		}
		else
		{
			SetSelectIndex(-1);
		}
	}

	ListBoxItem * ListBox::GetSelectItem()
	{
		return mSelectIndex != -1 ? mItems[mSelectIndex] : NULL;
	}

	void ListBox::Update(float elapsedTime)
	{
		if (mVisible && mAutoAdjustHeight)
		{
			Rect myRect = GetRect();

			myRect.h = mItemHeight * GetCount() + mClientRect.dy0 + mClientRect.dy1;

			SetRect(myRect);
		}

		Widget::Update(elapsedTime);
	}

	void ListBox::OnUpdate(float elapsedTime)
	{
		Rect myRect;
		myRect.x = 0;
		myRect.y = 0;
		myRect.w = GetClient().w;
		myRect.h = (float)mItemHeight;

		for (int i = 0; i < GetCount(); ++i)
		{
			ListBoxItem * item = mItems[i];

			Rect _rect = myRect;

			_rect.y += i * mItemHeight;

			item->SetRect(_rect);
		}

		if (mVScrollBar)
			mVScrollBar->SetStepValue(mItemHeight);

		Panel::OnUpdate(elapsedTime);
	}

	void ListBox::SerializeXML(xml_node root, bool is_out)
	{
		Widget::SerializeXML(root, is_out);

		if (is_out)
		{
			// ItemHeight
			String str;
			str.Format("%d", (int)GetItemHeight());

			root.append_node("ItemHeight").append_attribute("value", str.c_str());

			// Font
			if (mFont != NULL && mFont != FontManager::Instance()->GetDefaultFont())
			{
				root.append_node("Font").append_attribute("value", GetFontName().c_str());
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
		}
	}

}}