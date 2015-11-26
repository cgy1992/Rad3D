/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyGrid.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldComboBox : public PropertyField
	{
	public:
		PropertyFieldComboBox(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldComboBox();

	protected:
		MGUI::ComboBox * mComboBox;
	};

}
