#include "MGUI_Fade.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(FadeIn, Component);

	FadeIn::FadeIn()
		: mFadeTime(1)
		, mInternalTime(0)
	{
	}

	FadeIn::~FadeIn()
	{
	}

	int FadeIn::Update(float elapsedTime)
	{
		mInternalTime += elapsedTime;

		if (mInternalTime < mFadeTime)
		{
			float k = mInternalTime / mFadeTime;

			GetWidget()->SetAlpha(k);
		}
		else
		{
			GetWidget()->SetAlpha(1);

			E_FadeEnd(this);

			return -1;
		}

		return 0;
	}

	void FadeIn::SetFadeTime(float time)
	{
		mFadeTime = time;
		mInternalTime = 0;
	}



	ImplementRTTI(FadeOut, Component);

	FadeOut::FadeOut()
		: mFadeTime(1)
		, mInternalTime(0)
	{
	}

	FadeOut::~FadeOut()
	{
	}

	int FadeOut::Update(float elapsedTime)
	{
		mInternalTime += elapsedTime;

		if (mInternalTime < mFadeTime)
		{
			float k = 1 - mInternalTime / mFadeTime;

			GetWidget()->SetAlpha(k);
		}
		else
		{
			GetWidget()->SetAlpha(0);

			E_FadeEnd(this);

			return -1;
		}

		return 0;
	}

	void FadeOut::SetFadeTime(float time)
	{
		mFadeTime = time;
		mInternalTime = 0;
	}

}}