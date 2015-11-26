/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyGrid.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldColor : public PropertyField
	{
	public:
		PropertyFieldColor(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldColor();

	protected:
		void OnClick(const MGUI::MouseEvent *);

		void OnEndDialog_(bool _ok);
		void OnPreview_();

	protected:
		MGUI::Widget * mWidget;

		Float4 mColor;
	};

}
