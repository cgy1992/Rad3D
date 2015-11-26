/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyFieldComboBox.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldClass : public PropertyFieldComboBox
	{
	public:
		PropertyFieldClass(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldClass();

	protected:
		void OnSelChanged(const MGUI::Event * e, int index);

	protected:
		Array<FixedString32> mClassNames;
	};

}

