/*
*	Button
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_TextBox.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Button : public Widget
	{
		DECLARE_RTTI();

	public:
		Button(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~Button();

		void 
			SetCaption(const UString & caption);
		const UString & 
			GetCaption() const;

		void 
			SetFontName(const FixedString32 & name);
		FixedString32
			GetFontName() const;
		Font *
			_getFont() { return mTextBox->GetFont(); }

		void 
			SetTextColor(const Float4 & color);
		const Float4 & 
			GetTextColor() const;

		virtual void 
			SerializeXML(xml_node root, bool is_out);

	protected:
		bool mMouseDown;
		TextBox * mTextBox;
	};

}}
