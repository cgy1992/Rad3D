/*
*	ComboBox
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_ListBox.h"
#include "MGUI_Button.h"
#include "MGUI_EditBox.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY ComboBox : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, int> E_SelectChanged;

	public:
		ComboBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ComboBox();

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

		void 
			SetSelectIndex(int _index, bool _sendEvent = true);
		int 
			GetSelectIndex() const;

		void
			SetText(int _index, const UString & _text);
		const UString & 
			GetText(int _index);
		
		void * 
			GetUserData(int _index);

		void 
			SetItemHeight(float _height);
		float 
			GetItemHeight() const;

		void 
			SetDropHeightAuto(bool _auto);
		bool 
			GetDropHeightAuto() const;

		void 
			SetDropHeight(float _height);
		float 
			GetDropHeight() const;
		
		void 
			SetDropDown(bool _down);
		bool 
			GetDropDown();

		void 
			SetFontName(const FixedString32 & name);
		FixedString32
			GetFontName();
		Font *
			_getFont() { return mListBox->_getFont(); }

		virtual void 
			OnUpdate(float elapsedTime);

		ListBox *
			_getListBox() { return mListBox; }

		virtual void 
			SerializeXML(xml_node root, bool is_out);

	protected:
		void 
			OnDrop(const MouseEvent * e);
		void 
			OnSelectChanged(const Event * e, int index);
		void 
			OnInputMouseDown(const MouseEvent * e);

		void 
			_drop(bool popuped);

	protected:
		Button * mBnDrop;
		EditBox * mEditBox;
		ListBox * mListBox;

		bool mDropHeightAuto;
		float mDropHeight;
		bool mDropDown;
		bool mPopuped;

		cListener1<ComboBox, const MouseEvent *> mListenerInputMouseDown;
	};

}}
