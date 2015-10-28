/*
*	TreeViewItem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_CheckBox.h"
#include "MGUI_ImageBox.h"
#include "MGUI_TextBox.h"

namespace Rad { namespace MGUI {

	class TreeView;

	class MGUI_ENTRY TreeViewItem : public Widget
	{
		DECLARE_RTTI();

		friend class TreeView;

	public:
		TreeViewItem(TreeView * _parent, TreeViewItem * _parentItem);
		virtual ~TreeViewItem();

		void 
			SetImage(const String & _skinName, const Rect & _uvRect);

		void 
			SetText(const UString & text);
		const UString & 
			GetText() const;

		void 
			SetFont(Font * font);
		Font * 
			GetFont();

		TreeViewItem * 
			Append(const UString & _text, void * _userData = NULL);
		TreeViewItem * 
			Insert(int _index, const UString & _text, void * _userData = NULL);
		void 
			Remove(int _index);
		void 
			Remove(TreeViewItem * _item);
		void 
			Clear();

		int 
			GetCount();
		TreeViewItem * 
			GetChildItem(int _index);
		TreeViewItem * 
			GetParentItem();
		int 
			GetLevel();

		bool 
			IsExpand();

		virtual void 
			OnUpdateWidgetState();

	protected:
		virtual void 
			OnMouseDown(int _id, float _x, float _y);

		void 
			OnExpandClicked(const Event * _e, bool _checked);

		void 
			_updateVisible();
		float 
			_updateRect(float _y);

	protected:
		TreeView * mTreeView;
		TreeViewItem * mParentItem;
		int mLevel;

		CheckBox * mExpand;
		
		Widget * mItemBox;
		ImageBox * mImageBox;
		TextBox * mTextBox;

		Array<TreeViewItem *> mChildItem;
	};

}}