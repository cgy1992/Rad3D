/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyFieldEditBox.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldFixedString : public PropertyFieldEditBox
	{
	public:
		PropertyFieldFixedString(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldFixedString();

	protected:
		void OnTextChanged(const MGUI::FocusEvent *);
	};


	class UTIL_ENTRY PropertyFieldString : public PropertyFieldEditBox
	{
	public:
		PropertyFieldString(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldString();

	protected:
		void OnTextChanged(const MGUI::FocusEvent *);
	};

}
