/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyFieldComboBox.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldBool : public PropertyFieldComboBox
	{
	public:
		PropertyFieldBool(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldBool();

		void OnSelChanged(const MGUI::Event * e, int index);
	};

}
