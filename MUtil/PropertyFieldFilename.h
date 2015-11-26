/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyFieldButton.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldFilename : public PropertyFieldButton
	{
	public:
		PropertyFieldFilename(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldFilename();

	protected:
		void OnClick(const MGUI::ClickEvent * e);

	protected:
		FixedString256 mFilter;
	};

}

