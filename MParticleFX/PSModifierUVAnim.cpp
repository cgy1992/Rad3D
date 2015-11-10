#include "PSModifierUVAnim.h"

namespace Rad {

	DF_PROPERTY_BEGIN(PS_ModifierUVAnim)
		DF_PROPERTY(PS_ModifierUVAnim, mUVRect, "", "UVRect", PT_Int2)
		DF_PROPERTY(PS_ModifierUVAnim, mRandom, "", "Random", PT_Bool)
		DF_PROPERTY(PS_ModifierUVAnim, mLooped, "", "Looped", PT_Bool)
		DF_PROPERTY(PS_ModifierUVAnim, mInterval, "", "Interval", PT_Float)
		DF_PROPERTY(PS_ModifierUVAnim, mParamIndex, "", "ParamIndex", PT_Int)
		DF_PROPERTY(PS_ModifierUVAnim, mRandomParamIndex, "", "RandomParamIndex", PT_Int)
	DF_PROPERTY_END()

	ImplementRTTI(PS_ModifierUVAnim, PS_Modifier);

	PS_ModifierUVAnim::PS_ModifierUVAnim()
		: mUVRect(1, 1)
		, mParamIndex(0)
		, mInterval(0.05f)
		, mLooped(true)
		, mRandom(true)
		, mRandomParamIndex(1)
		, mUVRectStep(1, 1)
	{
	}

	PS_ModifierUVAnim::~PS_ModifierUVAnim()
	{
	}

	void PS_ModifierUVAnim::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mUVRect")
		{
			SetUVRect(mUVRect);
		}
		else
		{
			PS_Modifier::OnPropertyChanged(p);
		}
	}

	void PS_ModifierUVAnim::SetUVRect(const Int2 & xy)
	{
		mUVRect.x = Max(1, xy.x);
		mUVRect.y = Max(1, xy.y);

		mUVRectStep.x = 1.0f / mUVRect.x;
		mUVRectStep.y = 1.0f / mUVRect.y;
	}

	RectF PS_ModifierUVAnim::GetUVRect(int x, int y) const
	{
		d_assert (x < mUVRect.x && y < mUVRect.y);

		RectF rect;
		rect.x1 = x * mUVRectStep.x;
		rect.y1 = y * mUVRectStep.y;
		rect.x2 = rect.x1 + mUVRectStep.x;
		rect.y2 = rect.y1 + mUVRectStep.y;

		return rect;
	}

	RectF PS_ModifierUVAnim::GetUVRect(int index) const
	{
		int y = index / mUVRect.x;
		int x = index % mUVRect.x;

		return GetUVRect(x, y);
	}

	void PS_ModifierUVAnim::Modify(Particle * p, float elapsedTime)
	{
		int pi = GetParamIndex();
		int ri = GetRandomParamIndex();
		float k = p->InitLife.x - p->Life;

		if (k - p->Param[pi].v_float > mInterval)
		{
			p->Param[pi].v_float += mInterval;

			if (mRandom)
			{
				p->UVRect = RandomUVRect();
			}
			else
			{
				int i = p->Param[ri].v_int + 1;
				int count = GetUVRectCount();

				if (mLooped)
				{
					if (i >= count)
						i = 0;

					p->UVRect = GetUVRect(i);
				}
				else if (i < count)
				{
					p->UVRect = GetUVRect(i);
					p->Param[ri].v_int = i;
				}

				p->Param[ri].v_int = Min(i, count);
			}
		}
	}

	RectF PS_ModifierUVAnim::RandomUVRect()
	{
		int x = 0, y = 0;

		if (mRandom)
		{
			x = Math::RandRange(0, mUVRect.x - 1);
			y = Math::RandRange(0, mUVRect.y - 1);
		}

		return GetUVRect(x, y);
	}

	//
	DF_PROPERTY_BEGIN(PS_ModifierUVAnimKF)
		DF_PROPERTY(PS_ModifierUVAnimKF, mInterpolation, "", "Interpolation", PT_Bool)
		DF_PROPERTY_EX(PS_ModifierUVAnimKF, mKeyController, "", "KeyController", "PT_KeyController", PT_UserData)
	DF_PROPERTY_END()

	ImplementRTTI(PS_ModifierUVAnimKF, PS_ModifierKeyController);

	PS_ModifierUVAnimKF::PS_ModifierUVAnimKF()
		: mInterpolation(true)
	{
	}

	PS_ModifierUVAnimKF::~PS_ModifierUVAnimKF()
	{
	}

	void PS_ModifierUVAnimKF::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life * p->InitLife.y;

		KF_Float4 v;
		if (mKeyController.GetValue(v, time, mInterpolation))
		{
			p->UVRect.x1 = v.data.x;
			p->UVRect.y1 = v.data.y;
			p->UVRect.x2 = v.data.z;
			p->UVRect.y2 = v.data.w;
		}
	}
}