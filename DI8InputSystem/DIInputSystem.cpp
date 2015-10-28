#include "DIInputSystem.h"

#include "DIMouse.h"
#include "DIKeyboard.h"
#include "DIJoystick.h"

namespace Rad {

	LPDIRECTINPUT8	mDISystem;

	//
	ImplementSingleton(DIInputSystem);

	DIInputSystem::DIInputSystem(HINSTANCE hInst, HWND hWnd)
	{
		mKeyboard = NULL;
		mMouse = NULL;
		mJoystick = NULL;

		//create input
		HRESULT hr = S_OK;
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDISystem, NULL);
		if (FAILED(hr))
		{
			d_assert(0 && "Create Input Device Failed.");
		}
		else
		{
			mKeyboard = new DIKeyboard(mDISystem, hWnd); 
			mMouse = new DIMouse(mDISystem, hWnd);
			mJoystick = new DIJoystick(mDISystem, hWnd);
		}
	}

	DIInputSystem::~DIInputSystem()
	{
		//release object
		safe_delete(mKeyboard);
		safe_delete(mMouse);
		safe_delete(mJoystick);

		if (mDISystem)
		{
			mDISystem->Release();
			mDISystem = NULL;
		}
	}


	void DIInputSystem::Update()
	{
		if (mMouse) 
			static_cast<DIMouse *>(mMouse)->Update();

		if (mKeyboard)
			static_cast<DIKeyboard *>(mKeyboard)->Update();

		if (mJoystick)
			static_cast<DIJoystick *>(mJoystick)->Update();
	}

}
