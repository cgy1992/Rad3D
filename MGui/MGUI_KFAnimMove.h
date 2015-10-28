/*
*	KFAnimMove
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY KFAnimMove : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent2<Component *, int> E_KeyChanged;
		tEvent1<Component *> E_KeyEnd;

	public:
		KFAnimMove();
		virtual ~KFAnimMove();

		virtual void 
			Attach(ComponentOwner * owner);
		virtual void 
			Detach();

		virtual int
			Update(float elapsedTime);

		void
			Append(float time, float x, float y);
		
		KF_Animation<KF_Float2> *
			GetAnimation() { return &mAnimation; }

	protected:
		void 
			_move(float x, float y);

	protected:
		Rect mRect;
		KF_Animation<KF_Float2> mAnimation;
		int mKeyIndex;
	};

}}
