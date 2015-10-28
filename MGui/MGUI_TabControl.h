/*
*	TabControl
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_TabItem.h"
#include "MGUI_TabPage.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY TabControl : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent1<int> E_SelectChanged;

	public:
		TabControl(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~TabControl();

		void 
			Append(const UString & _text, void * _userData = NULL);
		void 
			Insert(int _index, const UString & _text, void * _userData = NULL);
		void 
			Remove(int _index);
		void 
			Clear();

		int 
			GetCount();
		const UString & 
			GetText(int _index);
		void * 
			GetUserData(int _index);
		TabItem * 
			GetItem(int _index);

		Widget *
			GetPage(int _index);

		void 
			SetSelectIndex(int _index);
		int 
			GetSelectIndex() const;
		TabItem * 
			GetSelectItem();
		TabPage * 
			GetSelectPage();

		virtual void 
			OnUpdate(float elapsedTime);
		virtual void 
			UpdateRenderItem(Layout * layout);

	protected:
		void 
			_updateItemRect();
		void 
			OnItemClick(const Event * e);

	protected:
		const LookFeel * mItemLookFeel;
		const LookFeel * mPageLookFeel;

		Array<TabItem *> mItems;
		Array<TabPage *> mPages;

		int mSelectIndex;
	};

}}
