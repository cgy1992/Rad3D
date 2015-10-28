#include "PSModifierUVScroll.h"

namespace Rad {

	DF_PROPERTY_BEGIN(PS_ModifierUVScroll)
		DF_PROPERTY(PS_ModifierUVScroll, mURate, "", "URate", PT_Float)
		DF_PROPERTY(PS_ModifierUVScroll, mVRate, "", "VRate", PT_Float)
	DF_PROPERTY_END()

	ImplementRTTI(PS_ModifierUVScroll, PS_Modifier);

	PS_ModifierUVScroll::PS_ModifierUVScroll()
		: mURate(0)
		, mVRate(0)
	{
	}

	PS_ModifierUVScroll::~PS_ModifierUVScroll()
	{
	}

	void PS_ModifierUVScroll::Modify(Particle * p, float elapsedTime)
	{
		float u = mURate * elapsedTime;
		float v = mVRate * elapsedTime;

		p->UVRect.x1 += u;
		p->UVRect.y1 += v;
		p->UVRect.x2 += u;
		p->UVRect.y2 += v;
	}

	//
	DF_PROPERTY_BEGIN(PS_ModifierUVScroll2)
		DF_PROPERTY(PS_ModifierUVScroll2, mURate, "", "URate", PT_Float)
		DF_PROPERTY(PS_ModifierUVScroll2, mVRate, "", "VRate", PT_Float)
		DF_PROPERTY(PS_ModifierUVScroll2, mTime2, "", "Time2", PT_Float)
		DF_PROPERTY(PS_ModifierUVScroll2, mURate2, "", "URate", PT_Float)
		DF_PROPERTY(PS_ModifierUVScroll2, mVRate2, "", "VRate", PT_Float)
	DF_PROPERTY_END()

	ImplementRTTI(PS_ModifierUVScroll2, PS_Modifier);

	PS_ModifierUVScroll2::PS_ModifierUVScroll2()
		: mURate(0)
		, mVRate(0)
		, mTime2(1)
		, mURate2(0)
		, mVRate2(0)
	{
	}

	PS_ModifierUVScroll2::~PS_ModifierUVScroll2()
	{
	}

	void PS_ModifierUVScroll2::Modify(Particle * p, float elapsedTime)
	{
		float time = 1 - p->Life / p->MaxLife;

		float u, v;

		if (time < mTime2)
		{
			u = mURate * elapsedTime;
			v = mVRate * elapsedTime;
		}
		else
		{
			u = mURate2 * elapsedTime;
			v = mVRate2 * elapsedTime;
		}

		p->UVRect.x1 += u;
		p->UVRect.y1 += v;
		p->UVRect.x2 += u;
		p->UVRect.y2 += v;
	}

}