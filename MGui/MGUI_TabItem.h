/*
*	TabItem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_TextBox.h"

namespace Rad { namespace MGUI {

	class TabControl;

	class MGUI_ENTRY TabItem : public Widget
	{
		DECLARE_RTTI();

	public:
		TabItem(const LookFeel * lookfeel, TabControl * parent, const UString & caption, void * usedata);
		virtual ~TabItem();

		void 
			SetText(const UString & caption);
		const UString & 
			GetText();

		void 
			SetFont(Font * font);
		Font * 
			GetFont();

		void 
			_updateRect();

		virtual void 
			OnUpdateWidgetState();

	protected:
		TextBox * mTextBox;
		TabControl * mTabControl;
	};

}}
