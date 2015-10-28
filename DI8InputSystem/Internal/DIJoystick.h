/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "DIInclude.h"
#include "MJoystick.h"

namespace Rad {

	class DIJoystick : public IJoystick
	{
	public:
		DIJoystick(LPDIRECTINPUT8 di, HWND hwnd);
		virtual ~DIJoystick();

		virtual bool KeyUp(int key);
		virtual bool KeyDown(int key);
		virtual bool KeyPressed(int key);

		virtual bool IsActive();

		//used to create the game controllers
		bool EnumDeviceCallBack(const DIDEVICEINSTANCE * inst, void * pData);

		void Update();


	protected:
		LPDIRECTINPUTDEVICE8	mDevice;
		DIJOYSTATE2				mGcState;
		DIJOYSTATE2             mOldState;
		char					mName[256];
		unsigned long           mNumButtons;

		//left and right stick x and y positions
		/*long m_xGCPos;
		long m_yGCPos;
		long m_xGCPos2;
		long m_yGCPos2;*/

		HWND					mhWnd;

		LPDIRECTINPUT8			mDISystem;

		bool					mActive;
	};

}