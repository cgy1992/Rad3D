#include "DIMouse.h"

namespace Rad {

DIMouse::DIMouse(LPDIRECTINPUT8 input, HWND hWnd)
{
	mActive = false;
	mDevice = NULL;
	mhWnd = NULL;
	ZeroMemory(&mMouseState, sizeof(DIMOUSESTATE));
	ZeroMemory(&mOldMouseState, sizeof(DIMOUSESTATE));

	DWORD flags;

	if (input->CreateDevice(GUID_SysMouse, & mDevice, NULL) == DI_OK)
	{
		if (mDevice->SetDataFormat(&c_dfDIMouse) == DI_OK)
		{
			/*
			if (exclusive)
				flags = DISCL_FOREGROUND | DISCL_EXCLUSIVE | DISCL_NOWINKEY;
			else
				flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
			*/

			flags = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

			if (FAILED(mDevice->SetCooperativeLevel(hWnd, flags)))
				mDevice->Acquire();

			mActive = true;
		}
	}

	mhWnd = hWnd;
}

DIMouse::~DIMouse()
{
	if (mDevice)
	{
		mDevice->Unacquire();
		mDevice->Release();
		mDevice = NULL;
	}
}

bool DIMouse::KeyUp(int key)
{
	if (!IsActive() && key < 0)
		return false;

	const BYTE * cur = mMouseState.rgbButtons;
	const BYTE * old = mOldMouseState.rgbButtons;

	return ((cur[key] & 0x80) != 0x80) && (cur[key] != old[key]);
}

bool DIMouse::KeyDown(int key)
{
	if (!IsActive())
		return false;

	const BYTE * cur = mMouseState.rgbButtons;
	const BYTE * old = mOldMouseState.rgbButtons;

	return ((cur[key] & 0x80) == 0x80) && (cur[key] != old[key]);
}

bool DIMouse::KeyPressed(int key)
{
	if (!IsActive())
		return false;

	const BYTE * cur = mMouseState.rgbButtons;
	const BYTE * old = mOldMouseState.rgbButtons;

	return ((cur[key] & 0x80) == 0x80) && (cur[key] == old[key]);
}

bool DIMouse::MouseMoved()
{
	if (!IsActive())
		return false;

    RECT rt;
    ::GetWindowRect(mhWnd, &rt);

    if (mPositionGlobal.x >= rt.left && mPositionGlobal.x <= rt.right &&
        mPositionGlobal.y >= rt.top && mPositionGlobal.y <= rt.bottom)
    {
        return mPositionGlobal.x != mOldPositionGlobal.x ||
               mPositionGlobal.y != mOldPositionGlobal.y;
    }

    return false;
}

int DIMouse::MouseWheel()
{
	if (!IsActive())
		return 0;

	return mMouseState.lZ;
}

bool DIMouse::IsActive()
{
	return mActive;
}

void DIMouse::Update()
{
	if (mDevice)
	{
		//save old state for input comparing
		memcpy(&mOldMouseState, &mMouseState, sizeof(mMouseState));

		HRESULT hr = S_OK;

		//if error getting device state, re-aquire
		hr = mDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);

		if (FAILED(hr))
		{
			hr = mDevice->Acquire();

			if (SUCCEEDED(hr))
				hr = mDevice->GetDeviceState(sizeof(DIMOUSESTATE), &mMouseState);
		}

		mActive = SUCCEEDED(hr);

		POINT pt;
		POINT ptLocal;
		::GetCursorPos(&pt);

		ptLocal = pt;

		::ScreenToClient(mhWnd, &ptLocal);

        mOldPositionLocal = mPositionLocal;
        mOldPositionGlobal = mPositionGlobal;

		mPositionGlobal.x = (float)pt.x;
		mPositionGlobal.y = (float)pt.y;
		mPositionLocal.x = (float)ptLocal.x;
		mPositionLocal.y = (float)ptLocal.y;
	}
}

}