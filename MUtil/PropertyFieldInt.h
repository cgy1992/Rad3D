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

	class UTIL_ENTRY PropertyFieldInt : public PropertyFieldEditBox
	{
	public:
		PropertyFieldInt(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldInt();

	protected:
		void OnTextChanged(const MGUI::FocusEvent *);
	};


	//
	class UTIL_ENTRY PropertyFieldInt2 : public PropertyFieldButton
	{
	public:
		PropertyFieldInt2(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldInt2();

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
	class UTIL_ENTRY PropertyFieldInt3 : public PropertyFieldButton
	{
	public:
		PropertyFieldInt3(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldInt3();

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
	class UTIL_ENTRY PropertyFieldInt4 : public PropertyFieldButton
	{
	public:
		PropertyFieldInt4(PropertyGroup * group, IObject * obj, const Property * prop);
		virtual ~PropertyFieldInt4();

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
