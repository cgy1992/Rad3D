/*
*	KFAnimSize
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY KFAnimSize : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent2<Component *, int> E_KeyChanged;
		tEvent1<Component *> E_KeyEnd;

	public:
		KFAnimSize();
		virtual ~KFAnimSize();

		virtual int
			Update(float elapsedTime);

		void
			Append(float time, float w, float h);

		KF_Animation<KF_Float2> *
			GetAnimation() { return &mAnimation; }

	protected:
		KF_Animation<KF_Float2> mAnimation;
		int mKeyIndex;
	};

}}