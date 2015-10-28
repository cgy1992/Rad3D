/*
*	FadeIn & FadeOut
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Component.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY FadeIn : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent1<Component *> E_FadeEnd;

	public:
		FadeIn();
		virtual ~FadeIn();

		virtual int
			Update(float elapsedTime);

		void 
			SetFadeTime(float time);

	protected:
		float mFadeTime;
		float mInternalTime;
	};

	class MGUI_ENTRY FadeOut : public Component
	{
		DECLARE_RTTI();

	public:
		tEvent1<Component *> E_FadeEnd;

	public:
		FadeOut();
		virtual ~FadeOut();

		virtual int
			Update(float elapsedTime);

		void 
			SetFadeTime(float time);

	protected:
		float mFadeTime;
		float mInternalTime;
	};

}}
