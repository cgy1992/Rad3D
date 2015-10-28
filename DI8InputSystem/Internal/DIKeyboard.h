/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "DIInclude.h"
#include "MKeyboard.h"

#define KEYS_SIZE 256

namespace Rad {

	class DIKeyboard : public IKeyboard
	{
	public:
		DIKeyboard(LPDIRECTINPUT8 di, HWND hWnd);
		virtual ~DIKeyboard();

		virtual bool KeyUp(int key);
		virtual bool KeyDown(int key);
		virtual bool KeyPressed(int key);

		virtual bool IsActive();

		void Update();

	protected:
		LPDIRECTINPUTDEVICE8 mDevice;
		char mKeys[KEYS_SIZE];
		char mOldKeys[KEYS_SIZE];
		bool mActive;
	};

}
