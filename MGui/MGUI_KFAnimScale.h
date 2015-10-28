/*
*	KFAnimScale
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY KFAnimScale : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent2<Component *, int> E_KeyChanged;
		tEvent1<Component *> E_KeyEnd;

	public:
		KFAnimScale();
		virtual ~KFAnimScale();

		virtual int
			Update(float elapsedTime);

		void
			Append(float time, float scale);

		KF_Animation<KF_Float> *
			GetAnimation() { return &mAnimation; }

	protected:
		KF_Animation<KF_Float> mAnimation;
		int mKeyIndex;
	};

}}
