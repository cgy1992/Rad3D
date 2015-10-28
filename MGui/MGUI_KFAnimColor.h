/*
*	KFAnimColor
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY KFAnimColor : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent2<Component *, int> E_KeyChanged;
		tEvent1<Component *> E_KeyEnd;

	public:
		KFAnimColor();
		virtual ~KFAnimColor();

		virtual int
			Update(float elapsedTime);

		void
			Append(float time, const Float4 & color);

		KF_Animation<KF_Float4> *
			GetAnimation () { return &mAnimation; }

	protected:
		KF_Animation<KF_Float4> mAnimation;
		int mKeyIndex;
	};

}}
