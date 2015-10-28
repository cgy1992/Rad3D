/*
*	Mouse
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat2.h"
#include "MSingleton.h"

namespace Rad {

	class M_ENTRY IMouse : public Singleton<IMouse>
	{
	public:
		IMouse();
		virtual ~IMouse();

		virtual bool 
			KeyUp(int key) = 0;
		virtual bool 
			KeyDown(int key) = 0;
		virtual bool
			KeyPressed(int key) = 0;

		virtual bool 
			MouseMoved() = 0;
		virtual int 
			MouseWheel() = 0;

		virtual bool 
			IsActive() = 0;

		Float2 
			GetPosition() { return mPositionLocal; };
		Float2 
			GetPositionGlobal() { return mPositionGlobal; };

		Float2 
			GetPositionDiff() { return Float2(mPositionLocal.x - mOldPositionLocal.x, mPositionLocal.y - mOldPositionLocal.y); };

	protected:
		Float2 mOldPositionLocal;
		Float2 mOldPositionGlobal;

		Float2 mPositionLocal;
		Float2 mPositionGlobal;
	};

}