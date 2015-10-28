/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMouse.h"
#include "MKeyboard.h"
#include "MJoystick.h"

#ifdef DI_EXPORT
	#define DI_ENTRY __declspec(dllexport)
#else
	#define DI_ENTRY __declspec(dllimport)
#endif

namespace Rad {

	class DI_ENTRY DIInputSystem : public Singleton<DIInputSystem>
	{
	public: 
		DIInputSystem(HINSTANCE hInst, HWND hWnd);
		~DIInputSystem();

		virtual void Update();

	protected:
		IKeyboard *	mKeyboard;
		IMouse * mMouse;
		IJoystick *	mJoystick;
	};

}
