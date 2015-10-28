/*
*	MGUI Event
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class Widget;

	struct Event
	{
		void * addr;
		Widget * sender;

		Event(void * p_addr) : addr(p_addr), sender(NULL) {}
	};

	//
	struct FocusEvent : public Event
	{
		Widget * widget;
		bool setOrLost;

		FocusEvent(void * p_addr) : Event(p_addr) {}
	};

	//
	struct MouseEvent : public Event
	{
		int id;
		float x, y, z;

		MouseEvent(void * p_addr) : Event(p_addr) {}
	};

	//
	struct KeyEvent : public Event
	{
		int key;
		uchar_t char_;

		KeyEvent(void * p_addr) : Event(p_addr) {}
	};

	//
	struct TouchEvent : public Event
	{
		int id;
		float x, y;

		TouchEvent(void * p_addr) : Event(p_addr) {}
	};

	//
	struct ClickEvent : public Event
	{
		int flag;
		float x, y;

		ClickEvent(void * p_addr) : Event(p_addr) {}
	};

}}
