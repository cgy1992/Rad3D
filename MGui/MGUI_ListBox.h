/*
*	ListBox
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Panel.h"
#include "MGUI_ListBoxItem.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY ListBox : public Panel
	{
		DECLARE_RTTI();

		friend class ListBoxItem;

	public:
		tEvent2<const Event *, int> E_SelectChanged;

	public:
		ListBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ListBox();

		ListBoxItem * 
			Append(const UString & _text, void * _userData = NULL);
		ListBoxItem * 
			Insert(int _index, const UString & _text, void * _userData = NULL);
		void 
			Remove(int _index);
		void 
			Clear();

		void
			SetText(int _index, const UString & _text);
		const UString & 
			GetText(int _index);
		int 
			GetCount();
		void * 
			GetUserData(int _index);
		ListBoxItem * 
			GetItem(int _index);

		void 
			SetItemHeight(float _height);
		float 
			GetItemHeight() const;

		void
			SetItemYOffset(float _yoff);
		float 
			GetItemYOffset() const;

		void 
			SetAutoAdjustHeight(bool _auto);
		bool 
			GetAutoAdjustHeight() const;

		void 
			SetFontName(const FixedString32 & name);
		FixedString32
			GetFontName() const;
		Font * 
			_getFont() { return mFont; }

		void 
			SetSelectIndex(int _index, bool _sendEvent = true);
		int 
			GetSelectIndex() const;
		void 
			SetSelectItem(ListBoxItem * _item);
		ListBoxItem * 
			GetSelectItem();

		virtual void
			Update(float elapsedTime);
		virtual void 
			OnUpdate(float elapsedTime);

		virtual void
			SerializeXML(xml_node root, bool is_out);

	protected:
		bool mAutoAdjustHeight;
		float mItemHeight;

		Font * mFont;
		const LookFeel * mItemLookFeel;
		Array<ListBoxItem *> mItems;
		int mSelectIndex;
	};

}}
