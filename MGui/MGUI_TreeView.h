/*
*	TreeView
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Panel.h"
#include "MGUI_TreeViewItem.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY TreeView : public Panel
	{
		DECLARE_RTTI();

		friend class TreeViewItem;

	public:
		tEvent1<TreeViewItem *> E_SelectChanged;

	public:
		TreeView(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~TreeView();

		void 
			SetFontName(const FixedString32 & name);
		FixedString32
			GetFontName() const;
		Font * 
			_getFont() { return mFont; }

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
			GetItem(int _index);

		void 
			SetItemHeight(float _height);
		float 
			GetItemHeight() const;

		void 
			SetSelectItem(TreeViewItem * _item);
		TreeViewItem * 
			GetSelectItem();

		virtual void 
			OnUpdate(float elapsedTime);

	protected:
		Font * mFont;

		float mItemHeight;
		Array<TreeViewItem *> mChildItem;
		TreeViewItem * mSelectItem;
	};

}}

