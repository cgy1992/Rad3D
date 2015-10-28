/*
*	CheckBox
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {


	class MGUI_ENTRY CheckBox : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, bool> E_Checked;

	public:
		CheckBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~CheckBox();

		void 
			SetChecked(bool checked) { mChecked = checked; }
		bool 
			GetChecked() const { return mChecked; }

		virtual void 
			OnUpdateWidgetState();

		virtual void
			SerializeXML(xml_node root, bool is_out);

	protected:
		virtual void 
			OnMouseUp(int _id, float _x, float _y);
		virtual void 
			OnTouchUp(int _id, float _x, float _y);

	protected:
		bool mChecked;
	};

}}
