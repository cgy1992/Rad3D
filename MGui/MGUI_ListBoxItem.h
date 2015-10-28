/*
*	ListBoxItem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once
 
#include "MGUI_Widget.h"
#include "MGUI_TextBox.h"

namespace Rad { namespace MGUI {

	class ListBox;

	class MGUI_ENTRY ListBoxItem : public Widget
	{
		DECLARE_RTTI();

		friend class ListBox;

	public:
		ListBoxItem(ListBox * _listBox, const LookFeel * _lookfeel,
			             const UString & _text, void * _userData);
		virtual ~ListBoxItem();

		void 
			SetText(const UString & text) { mTextBox->SetCaption(text); }
		const UString & 
			GetText() { return mTextBox->GetCaption(); }

		void 
			SetTextColor(const Float4 & color) { mTextBox->SetColor(color); }
		const Float4 & 
			GetTextColor() { return mTextBox->GetColor(); }

		void 
			SetFont(Font * font) { mTextBox->SetFont(font); }
		Font * 
			GetFont() { return mTextBox->GetFont(); }

		virtual void 
			OnUpdateWidgetState();

	protected:
		virtual void 
			OnMouseDown(int _id, float _x, float _y);
		virtual void 
			OnTouchDown(int _id, float _x, float _y);

	protected:
		ListBox * mListBox;
		TextBox * mTextBox;
	};
	
}}
