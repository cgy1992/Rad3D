/*
*	Keyboard
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"

namespace Rad {

	class M_ENTRY IKeyboard : public Singleton<IKeyboard>
	{
	public:
		IKeyboard();
		virtual ~IKeyboard();

		virtual bool 
			KeyUp(int key) = 0;
		virtual bool 
			KeyDown(int key) = 0;
		virtual bool 
			KeyPressed(int key) = 0;

		virtual bool 
			IsActive() = 0;
	};

}