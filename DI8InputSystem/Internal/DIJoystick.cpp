#include "DIJoystick.h"

#pragma warning(push)
#pragma warning(disable : 4018)

namespace Rad {

	DIJoystick * gThis = NULL;

	bool gJSEnumDeviceCallBack(const DIDEVICEINSTANCE * inst, void * pData)
	{
		return gThis->EnumDeviceCallBack(inst, pData);
	}

	DIJoystick::DIJoystick(LPDIRECTINPUT8 di, HWND hwnd)
	{
		mActive = false;

		mDevice = NULL;
		ZeroMemory(&mGcState, sizeof(DIJOYSTATE2));
		ZeroMemory(&mOldState, sizeof(DIJOYSTATE2));
		mName[0] = '\0';
		mNumButtons = 0;
		mhWnd = NULL;
	
		//Save copies
		gThis = this;
		mhWnd = hwnd;
		mDISystem = di;

		DIPROPRANGE range;
		DIDEVCAPS caps;

		mDISystem->EnumDevices(DI8DEVCLASS_GAMECTRL,
			(LPDIENUMDEVICESCALLBACK)gJSEnumDeviceCallBack, NULL, 
			DIEDFL_ATTACHEDONLY);

		if (mDevice)
		{
			range.diph.dwSize = sizeof(DIPROPRANGE);
			range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			range.diph.dwHow = DIPH_BYOFFSET;
			range.lMin = -1000;
			range.lMax = 1000;
			range.diph.dwObj = DIJOFS_X;
			mDevice->SetProperty(DIPROP_RANGE, &range.diph);
			range.diph.dwObj = DIJOFS_Y;
			mDevice->SetProperty(DIPROP_RANGE, &range.diph);

			if (SUCCEEDED(mDevice->GetCapabilities(&caps)))
				mNumButtons = caps.dwButtons;
			else
				mNumButtons = 4;

			mActive = true;
		}
	}

	DIJoystick::~DIJoystick()
	{
		if (mDevice)
		{
			mDevice->Unacquire();
			mDevice->Release();
			mDevice = NULL;
		}
	}


	bool DIJoystick::EnumDeviceCallBack(const DIDEVICEINSTANCE *inst, void *pData)
	{
		//set to the first device found
		if (SUCCEEDED(mDISystem->CreateDevice(inst->guidInstance, 
			&mDevice, NULL)))
		{
			if (SUCCEEDED(mDevice->SetDataFormat(&c_dfDIJoystick2)))
			{
				if (SUCCEEDED(mDevice->SetCooperativeLevel(mhWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
				{
					if (SUCCEEDED(mDevice->Acquire()))
					{
						strcpy(mName, (char*)inst->tszInstanceName);
						return DIENUM_STOP;
					}
				}
			}
		}

		//return continue tot try to init other connected devices
		return DIENUM_CONTINUE;
	}

	bool DIJoystick::KeyUp(int key)
	{
		if (key < 0 || key >= mNumButtons)
			return false;

		const BYTE * cur = mGcState.rgbButtons;
		const BYTE * old = mOldState.rgbButtons;

		return ((cur[key] & 0x80) != 0x80) && (cur[key] != old[key]);
	}

	bool DIJoystick::KeyDown(int key)
	{
		if (key < 0 || key >= mNumButtons)
			return false;

		const BYTE * cur = mGcState.rgbButtons;
		const BYTE * old = mOldState.rgbButtons;

		return ((cur[key] & 0x80) == 0x80) && (cur[key] != old[key]);
	}

	bool DIJoystick::KeyPressed(int key)
	{
		if (key < 0 || key >= mNumButtons)
			return false;

		const BYTE * cur = mGcState.rgbButtons;
		const BYTE * old = mOldState.rgbButtons;

		return ((cur[key] & 0x80) == 0x80) && (cur[key] == old[key]);
	}

	bool DIJoystick::IsActive()
	{
		return mActive;
	}

	void DIJoystick::Update()
	{
		if (mDevice)
		{
			mDevice->Poll();

			//save old state for input comparing
			memcpy(&mOldState, &mGcState, sizeof(mGcState));

			HRESULT hr = S_OK;

			hr = mDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mGcState);

			if (FAILED(hr))
			{
				hr = mDevice->Acquire();

				if (SUCCEEDED(hr))
				{
					hr = mDevice->GetDeviceState(sizeof(DIJOYSTATE2), &mGcState);
				}
			}

			mActive = SUCCEEDED(hr);
		}
	}

}

#pragma warning(pop)
