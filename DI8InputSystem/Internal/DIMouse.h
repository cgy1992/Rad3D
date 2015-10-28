/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "DIInclude.h"
#include "MMouse.h"

namespace Rad {

	class DIMouse : public IMouse
	{
	public:
		DIMouse(LPDIRECTINPUT8 input, HWND hwnd);
		virtual ~DIMouse();

		virtual bool KeyUp(int key);
		virtual bool KeyDown(int key);
		virtual bool KeyPressed(int key);

		virtual bool MouseMoved();
		virtual int MouseWheel();

		virtual bool IsActive();

		void Update();

	protected:
		LPDIRECTINPUTDEVICE8 mDevice;

		DIMOUSESTATE mMouseState;
		DIMOUSESTATE mOldMouseState;
		HWND mhWnd;

		bool mActive;
	};

}