#include "MGUI_TreeViewItem.h"
#include "MGUI_TreeView.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(TreeViewItem, Widget);

	TreeViewItem::TreeViewItem(TreeView * _parent, TreeViewItem * _parentItem)
		: Widget(NULL, _parent->GetClientWidget())
		, mTreeView(_parent)
		, mParentItem(_parentItem)
	{
		mLevel = _parentItem != NULL ? _parentItem->GetLevel() + 1 : 0;

		mExpand = new CheckBox(_parent->GetLookFeel()->GetChild("TreeViewExpand"), this);
		mExpand->SetAlign(eAlign::V_CENTER);

		mItemBox = new Widget(_parent->GetLookFeel()->GetChild("TreeViewItem"), this);
		mItemBox->SetPickFlag(ePickFlag::NONE);
		mItemBox->SetInheritState(true);

		mImageBox = new ImageBox(NULL, mItemBox);
		mImageBox->SetAlign(eAlign::V_CENTER);

		mTextBox = new TextBox(mItemBox->GetLookFeel(), mItemBox);
		mTextBox->SetAlign(eAlign::V_CENTER);
		mTextBox->SetPickFlag(ePickFlag::NONE);
		mTextBox->SetInheritState(true);
		mTextBox->SetFont(_parent->_getFont());

		mExpand->E_Checked += new cListener2<TreeViewItem, const Event *, bool>(this, &TreeViewItem::OnExpandClicked);
	}

	TreeViewItem::~TreeViewItem()
	{
		Clear();
	}

	void TreeViewItem::SetImage(const String & _skinName, const Rect & _uvRect)
	{
		mImageBox->SetSkin(_skinName);
		mImageBox->SetUVRect(_uvRect);
	}

	void TreeViewItem::SetText(const UString & text)
	{
		mTextBox->SetCaption(text);
	}

	const UString & TreeViewItem::GetText() const
	{
		return mTextBox->GetCaption();
	}

	void TreeViewItem::SetFont(Font * font)
	{
		mTextBox->SetFont(font);
	}

	Font * TreeViewItem::GetFont()
	{
		return mTextBox->GetFont();
	}

	TreeViewItem * TreeViewItem::Append(const UString & _text, void * _userData)
	{
		return Insert(GetCount(), _text, _userData);
	}

	TreeViewItem * TreeViewItem::Insert(int _index, const UString & _text, void * _userData)
	{
		TreeViewItem * pNewItem = new TreeViewItem(mTreeView, this);

		pNewItem->SetText(_text);
		pNewItem->SetUserData(_userData);
		pNewItem->SetVisible(mExpand->GetChecked());

		mChildItem.Insert(_index, pNewItem);

		return pNewItem;
	}

	void TreeViewItem::Remove(int _index)
	{
		delete mChildItem[_index];
		mChildItem.Erase(_index);
	}

	void TreeViewItem::Remove(TreeViewItem * _item)
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

	void TreeViewItem::Clear()
	{
		for (int i = 0; i < mChildItem.Size(); ++i)
			delete mChildItem[i];
		
		mChildItem.Clear();
	}

	int TreeViewItem::GetCount()
	{
		return mChildItem.Size();
	}

	TreeViewItem * TreeViewItem::GetChildItem(int _index)
	{
		return mChildItem[_index];
	}

	TreeViewItem * TreeViewItem::GetParentItem()
	{
		return mParentItem;
	}

	int TreeViewItem::GetLevel()
	{
		return mLevel;
	}

	bool TreeViewItem::IsExpand()
	{
		return mExpand->GetChecked();
	}

	void TreeViewItem::OnUpdateWidgetState()
	{
		Widget::OnUpdateWidgetState();

		if (mTreeView->IsEnable() && mTreeView->GetSelectItem() == this)
			mState = eWidgetState::SELECT;
	}

	void TreeViewItem::OnMouseDown(int _id, float _x, float _y)
	{
		if (_id == InputCode::MKC_LEFT)
		{
			mTreeView->SetSelectItem(this);
		}
	}

	void TreeViewItem::OnExpandClicked(const Event * _e, bool _checked)
	{
		for (int i = 0; i < mChildItem.Size(); ++i)
		{
			mChildItem[i]->SetVisible(_checked);
			mChildItem[i]->_updateVisible();
		}
	}

	void TreeViewItem::_updateVisible()
	{
		if (mParentItem == NULL)
			return ;

		if (mParentItem->IsExpand() && mParentItem->IsVisible())
		{
			SetVisible(true);
		}
		else
		{
			SetVisible(false);
		}

		for (int i = 0; i < mChildItem.Size(); ++i)
		{
			mChildItem[i]->_updateVisible();
		}
	}

	float TreeViewItem::_updateRect(float _y)
	{
		Rect rect;
		rect.x = 0;
		rect.y = _y;
		rect.w = mTreeView->GetClient().w;
		rect.h = mTreeView->GetItemHeight();

		SetRect(rect);

		_y += rect.h;

		_updateTM();

		// update expand
		float x = (mExpand->GetRect().w + 3) * mLevel;
		mExpand->SetRectPosition(x, mExpand->GetRect().y);
		mExpand->SetVisible(mChildItem.Size() > 0);

		// update item box
		{
			Rect myRect;
			myRect.x = mExpand->GetRect().x + mExpand->GetRect().w + 3;
			myRect.w = mImageBox->GetRect().w + mTextBox->GetRect().w;
			myRect.y = 0;
			myRect.h = rect.h;

			myRect.w += mItemBox->GetClientRect().dx0 + mItemBox->GetClientRect().dx1;

			if (mImageBox->GetRect().w > 0)
				myRect.w += 3;

			mItemBox->SetRect(myRect);
		}

		// update text box
		{
			float x = 0;

			if (mImageBox->GetRect().w > 0)
				x += mImageBox->GetRect().w + 3;

			mTextBox->SetRectPosition(x, mTextBox->GetRect().y);
		}

		// update child
		if (mExpand->GetChecked())
		{
			for (int i = 0; i < mChildItem.Size(); ++i)
			{
				_y = mChildItem[i]->_updateRect(_y);
			}

			int __i = 0;
		}

		return _y;
	}

}}