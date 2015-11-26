/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyFieldButton.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldLongText : public PropertyFieldButton
	{
	public:
		PropertyFieldLongText(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldLongText();

	protected:
		void OnClick(const MGUI::ClickEvent * e);

		void OnOK(const MGUI::ClickEvent * e);
		void OnCancel(const MGUI::ClickEvent * e);
		void OnCompile(const MGUI::ClickEvent * e);

		void OnLibChanged(const MGUI::Event * e, int index);
		void OnFunctionChanged(const MGUI::Event * e, int index);

	protected:
		MGUI::Layout * mDialog;
		MGUI::EditView * mEditView;
		MGUI::ComboBox * mLibBox;
		MGUI::ListBox * mFunctionBox;
		MGUI::Label * mStatusLabel;
	};

}

