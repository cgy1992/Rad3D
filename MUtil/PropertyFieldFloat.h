/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"
#include "PropertyFieldEditBox.h"
#include "PropertyFieldButton.h"

namespace Rad {

	class UTIL_ENTRY PropertyFieldFloat : public PropertyFieldEditBox
	{
	public:
		PropertyFieldFloat(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldFloat();

	protected:
		void 
			OnTextChanged(const MGUI::FocusEvent *);
	};

	//
	class UTIL_ENTRY PropertyFieldFloat2 : public PropertyFieldButton
	{
	public:
		PropertyFieldFloat2(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldFloat2();

	protected:
		void 
			OnClick(const MGUI::ClickEvent * e);
		void 
			OnOK(const MGUI::ClickEvent * e);
		void 
			OnCancel(const MGUI::ClickEvent * e);

	protected:
		MGUI::Layout * mLayout;
		MGUI::EditBox * mEditBoxX;
		MGUI::EditBox * mEditBoxY;
	};


	//
	class UTIL_ENTRY PropertyFieldFloat3 : public PropertyFieldButton
	{
	public:
		PropertyFieldFloat3(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldFloat3();

	protected:
		void 
			OnClick(const MGUI::ClickEvent * e);
		void 
			OnOK(const MGUI::ClickEvent * e);
		void 
			OnCancel(const MGUI::ClickEvent * e);

	protected:
		MGUI::Layout * mLayout;

		MGUI::EditBox * mEditBoxX;
		MGUI::EditBox * mEditBoxY;
		MGUI::EditBox * mEditBoxZ;
	};


	//
	class UTIL_ENTRY PropertyFieldFloat4 : public PropertyFieldButton
	{
	public:
		PropertyFieldFloat4(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldFloat4();

	protected:
		void 
			OnClick(const MGUI::ClickEvent * e);
		void 
			OnOK(const MGUI::ClickEvent * e);
		void 
			OnCancel(const MGUI::ClickEvent * e);

	protected:
		MGUI::Layout * mLayout;

		MGUI::EditBox * mEditBoxX;
		MGUI::EditBox * mEditBoxY;
		MGUI::EditBox * mEditBoxZ;
		MGUI::EditBox * mEditBoxW;
	};

}

