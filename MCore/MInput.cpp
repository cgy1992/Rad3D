#include "MInput.h"
#include "MMouse.h"
#include "MKeyboard.h"
#include "MJoystick.h"

namespace Rad {

	ImplementSingleton(IMouse);

	IMouse::IMouse()
	{
	}

	IMouse::~IMouse()
	{
	}

	//
	ImplementSingleton(IKeyboard);

	IKeyboard::IKeyboard()
	{
	}
	
	IKeyboard::~IKeyboard()
	{
	}

	//
	ImplementSingleton(IJoystick);

	IJoystick::IJoystick()
	{
	}

	IJoystick::~IJoystick()
	{
	}
}