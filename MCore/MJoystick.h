/*
*	Joystick
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"

namespace Rad {

	class M_ENTRY IJoystick : public Singleton<IJoystick>
	{
	public:
		IJoystick();
		virtual ~IJoystick();

		virtual bool 
			KeyUp(int key) = 0;
		virtual bool 
			KeyDown(int key) = 0;

		virtual bool 
			KeyPressed(int key) = 0;

		virtual bool 
			IsActive() = 0;
	};

}