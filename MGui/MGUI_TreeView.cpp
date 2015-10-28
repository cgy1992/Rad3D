#include "MGUI_TreeView.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(TreeView, Panel);

	TreeView::TreeView(const LookFeel * _lookfeel, Widget * _parent)
		: Panel(_lookfeel, _parent)
		, mSelectItem(NULL)
		, mItemHeight(24)
	{
		d_assert (_lookfeel != NULL);

		mFont = FontManager::Instance()->GetDefaultFont();
	}

	TreeView::~TreeView()
	{
		E_SelectChanged.DetachAll();

		Clear();
	}

	void TreeView::SetFontName(const FixedString32 & name)
	{
		Font * p = FontManager::Instance()->GetFont(name);
		if (p)
		{
			mFont = p;
		}
	}

	FixedString32 TreeView::GetFontName() const
	{
		Font * font = mFont;
		if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
		{
			return font->GetName();
		}

		return "";
	}

	TreeViewItem * TreeView::Append(const UString & _text, void * _userData)
	{
		return Insert(GetCount(), _text, _userData);
	}

	TreeViewItem * TreeView::Insert(int _index, const UString & _text, void * _userData)
	{
		TreeViewItem * pNewItem = new TreeViewItem(this, NULL);

		pNewItem->SetText(_text);
		pNewItem->SetUserData(_userData);

		mChildItem.Insert(_index, pNewItem);

		return pNewItem;
	}

	void TreeView::Remove(int _index)
	{
		if (mSelectItem == mChildItem[_index])
		{
			SetSelectItem(NULL);
		}

		delete mChildItem[_index];

		mChildItem.Erase(_index);
	}

	void TreeView::Remove(TreeViewItem * _item)
	{
		d_assert (_item != NULL);

		if (mSelectItem == _item)
		{
			SetSelectItem(NULL);
		}

		if (_item->GetParentItem())
		{
			_item->GetParentItem()->Remove(_item);
		}
		else
		{
			for (int i = 0; i < mChildItem.Size(); ++i)
			{
				if (mChildItem[i] == _item)
				{
					delete _item;
					mChildItem.Erase(i);
					return ;
				}
			}

			d_assert (0);
		}
	}

	void TreeView::Clear()
	{
		SetSelectItem(NULL);

		for (int i = 0; i < mChildItem.Size(); ++i)
		{
			delete mChildItem[i];
		}

		mChildItem.Clear();
	}

	int TreeView::GetCount()
	{
		return mChildItem.Size();
	}

	TreeViewItem * TreeView::GetItem(int _index)
	{
		return mChildItem[_index];
	}

	void TreeView::SetItemHeight(float _height)
	{
		mItemHeight = _height;
	}

	float TreeView::GetItemHeight() const
	{
		return mItemHeight;
	}

	void TreeView::SetSelectItem(TreeViewItem * _item)
	{
		if (mSelectItem != _item)
		{
			mSelectItem = _item;
			E_SelectChanged(mSelectItem);
		}
	}

	TreeViewItem * TreeView::GetSelectItem()
	{
		return mSelectItem;
	}

	void TreeView::OnUpdate(float elapsedTime)
	{
		float y = 0;

		for (int i = 0; i < GetCount(); ++i)
		{
			y = mChildItem[i]->_updateRect(y);
		}

		if (mVScrollBar)
			mVScrollBar->SetStepValue(mItemHeight);

		Panel::OnUpdate(elapsedTime);
	}

}}