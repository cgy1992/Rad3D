#include "DIKeyboard.h"

namespace Rad
{

//keyboard
DIKeyboard::DIKeyboard(LPDIRECTINPUT8 input, HWND hwnd)
{
	mDevice = NULL;
	ZeroMemory(mKeys, sizeof(char) * KEYS_SIZE);
	ZeroMemory(mOldKeys, sizeof(char) * KEYS_SIZE);
	mActive = false;


	//init the keyboard
	if (input->CreateDevice(GUID_SysKeyboard, &mDevice, NULL) == DI_OK)
	{
		if (SUCCEEDED(mDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			//set cooperative level
			if (mDevice->SetCooperativeLevel(hwnd, 
					DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) == DI_OK)
			{
				mDevice->Acquire();
				mActive = true;
			}
		}
	}

	memset(mKeys, 0, sizeof(mKeys));
}

DIKeyboard::~DIKeyboard()
{
	if (mDevice)
	{
		mDevice->Unacquire();
		mDevice->Release();
		mDevice = NULL;
	}
}

bool DIKeyboard::KeyUp(int key)
{
	return ((mKeys[key] & 0x80) != 0x80) && (mKeys[key] != mOldKeys[key]);
}

bool DIKeyboard::KeyDown(int key)
{
	return ((mKeys[key] & 0x80) == 0x80) && (mKeys[key] != mOldKeys[key]);
}

bool DIKeyboard::KeyPressed(int key)
{
	return ((mKeys[key] & 0x80) == 0x80) && (mKeys[key] == mOldKeys[key]);
}

bool DIKeyboard::IsActive()
{
	return mDevice != NULL;
}

void DIKeyboard::Update()
{
	if (mDevice)
	{
		//save old state for input comparing
		memcpy(mOldKeys, mKeys, sizeof(mKeys));

		// if error getting device state, re-aquire
		HRESULT hr = S_OK;
		hr = mDevice->GetDeviceState(sizeof(mKeys), (LPVOID)mKeys);
		if (FAILED(hr))
		{
			hr = mDevice->Acquire();
			if (SUCCEEDED(hr))
			{
				hr = mDevice->GetDeviceState(sizeof(mKeys), (LPVOID)mKeys);
			}
		}

		mActive = SUCCEEDED(hr);
	}

}

}