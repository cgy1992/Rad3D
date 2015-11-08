#include "MGUI_TabControl.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(TabControl, Widget);

	TabControl::TabControl(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mItemLookFeel = _lookfeel->GetChild("Item");
		mPageLookFeel = _lookfeel->GetChild("Page");
		mSelectIndex = -1;
	}

	TabControl::~TabControl()
	{
	}

	void TabControl::Append(const UString & _text, void * _userData)
	{
		mItems.PushBack(new TabItem(mItemLookFeel, this, _text, _userData));
		mPages.PushBack(new TabPage(mPageLookFeel, this));
	}

	void TabControl::Insert(int _index, const UString & _text, void * _userData)
	{
		mItems.Insert(_index, new TabItem(mItemLookFeel, this, _text, _userData));
		mPages.Insert(_index, new TabPage(mPageLookFeel, this));

		if (mSelectIndex >= _index)
			SetSelectIndex(mSelectIndex + 1);
	}

	void TabControl::Remove(int _index)
	{
		delete mItems[_index];
		mItems.Erase(_index);

		delete mPages[_index];
		mPages.Erase(_index);

		if (mSelectIndex >= _index)
			SetSelectIndex(mSelectIndex - 1);
	}

	void TabControl::Clear()
	{
		SetSelectIndex(-1);

		for (int i = 0; i < mItems.Size(); ++i)
			delete mItems[i];

		for (int i = 0; i < mPages.Size(); ++i)
			delete mPages[i];

		mItems.Clear();
		mPages.Clear();
	}

	int TabControl::GetCount()
	{
		return mItems.Size();
	}

	const UString & TabControl::GetText(int _index)
	{
		return mItems[_index]->GetText();
	}

	void * TabControl::GetUserData(int _index)
	{
		return mItems[_index]->GetUserData();
	}

	TabItem * TabControl::GetItem(int _index)
	{
		return mItems[_index];
	}

	Widget * TabControl::GetPage(int _index)
	{
		return mPages[_index];
	}

	void TabControl::SetSelectIndex(int _index, bool _sendEvent)
	{
		d_assert (_index < GetCount());

		if (_index != mSelectIndex)
		{
			mSelectIndex = _index;

			if (_sendEvent)
			{
				E_SelectChanged(_index);
			}
		}
	}

	int TabControl::GetSelectIndex() const
	{
		return mSelectIndex;
	}

	TabItem * TabControl::GetSelectItem()
	{
		return mSelectIndex != -1 ? mItems[mSelectIndex] : NULL;
	}

	TabPage * TabControl::GetSelectPage()
	{
		return mSelectIndex != -1 ? mPages[mSelectIndex] : NULL;
	}


	void TabControl::OnUpdate(float elapsedTime)
	{
		_updateItemRect();

		for (int i = 0; i < mPages.Size(); ++i)
		{
			mPages[i]->SetVisible(false);
		}

		if (mSelectIndex != -1)
		{
			mPages[mSelectIndex]->SetVisible(true);
		}
	}

	void TabControl::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}

	void TabControl::_updateItemRect()
	{
		for (int i = 0; i < mItems.Size(); ++i)
			mItems[i]->_updateRect();

		float offset = 0;
		for (int i = 0; i < mItems.Size(); ++i)
		{
			mItems[i]->SetRectPosition(offset, 0);
			offset = mItems[i]->GetRect().w;
		}

		Rect pgRect = mRect;

		for (int i = 0; i < mPages.Size(); ++i)
		{
			pgRect.y += mItems[i]->GetRect().h;
			pgRect.h -= mItems[i]->GetRect().h;

			mPages[i]->SetRect(pgRect);
		}
	}

	void TabControl::OnItemClick(const Event * e)
	{
		for (int i = 0; i < mItems.Size(); ++i)
		{
			if (mItems[i] == e->sender)
			{
				SetSelectIndex(i);
				break;
			}
		}
	}

}}