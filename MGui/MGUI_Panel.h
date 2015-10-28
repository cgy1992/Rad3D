/*
*	Panel
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_ScrollBar.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Panel : public Widget
	{
		DECLARE_RTTI();

	public:
		Panel(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~Panel();

		Widget * 
			GetClientWidget() { return mClientWidget; }

		void 
			SetHScrollBar(const LookFeel * _lookfeel);
		void 
			SetVScrollBar(const LookFeel * _lookfeel);

		virtual void 
			OnUpdate(float elapsedTime);

	protected:
		void 
			_updateClient();
		void 
			_updateVScroll();
		void 
			_updateHScroll();

	protected:
		void 
			OnHScroll(const Event *, float _position);
		void 
			OnVScroll(const Event *, float _position);

	protected:
		Widget * mClientWidget;
		ScrollBarV * mVScrollBar;
		ScrollBarH * mHScrollBar;
	};

}}
