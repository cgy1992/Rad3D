#pragma once

#include "stdafx.h"

class IME
{
public:
	static void OpenIMEKeyboard();
	static void CloseIMEKeyboard();

public:
	IME();
	~IME();

	void OnOpenIMEKeyboard();
	void OnCloseIMEKeyboard();

public:
	bool bOpenKeyboard;

protected:
	cListener0<IME> L_OpenIMEKeyboard;
	cListener0<IME> L_CloseIMEKeyboard;
};

