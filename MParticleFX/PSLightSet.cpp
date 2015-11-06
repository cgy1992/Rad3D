#include "PSLightSet.h"

namespace Rad {

	ImplementRTTI(PS_LightSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_LightSet)
		DF_PROPERTY_ENUM(PS_LightSet, mLightType, "Light", "Type", eLightType)
		DF_PROPERTY(PS_LightSet, mAttenStart, "Attenuation", "Start", PT_Float)
		DF_PROPERTY(PS_LightSet, mAttenEnd, "Attenuation", "End", PT_Float)
		DF_PROPERTY(PS_LightSet, mAttenFallOff, "Attenuation", "FallOff", PT_Float)
		DF_PROPERTY(PS_LightSet, mSpotInner, "SpotParam", "Inner", PT_Float)
		DF_PROPERTY(PS_LightSet, mSpotOuter, "SpotParam", "Outer", PT_Float)
		DF_PROPERTY(PS_LightSet, mSpotFallOff, "SpotParam", "FallOff", PT_Float)
	DF_PROPERTY_END()

	PS_LightSet::PS_LightSet()
		: mLightType(eLightType::POINT)
		, mAttenStart(0)
		, mAttenEnd(1 * UNIT_METRES)
		, mAttenFallOff(1)
		, mSpotInner(1)
		, mSpotOuter(0.7071f)
		, mSpotFallOff(1)
	{
	}

	PS_LightSet::~PS_LightSet()
	{
		_clear_particle();
	}

	void PS_LightSet::AllocQuota(int quota)
	{
	}

	void PS_LightSet::AddRenderQueue(RenderQueue * rq)
	{
	}

	void PS_LightSet::Update(float elapsedTime)
	{
		PS_Set::Update(elapsedTime);

		if (mParticles.Size() > 0)
		{
			mBound = Aabb::Invalid;

			for (int i = 0; i < mParticles.Size(); ++i)
			{
				Particle * p = mParticles[i];

				if (p->InternalData)
				{
					PS_Light * data = (PS_Light *)p->InternalData;

					data->_update(p);
				}

				Float3 vMin = p->Position - p->Size * 0.5f;
				Float3 vMax = p->Position + p->Size * 0.5f;

				mBound.minimum = Float3::Minimum(mBound.minimum, vMin);
				mBound.maximum = Float3::Maximum(mBound.maximum, vMax);
			}

			if (IsLocalSpace())
			{
				mBound.minimum += mParent->GetWorldPosition();
				mBound.maximum += mParent->GetWorldPosition();
			}
		}
	}

	Particle * PS_LightSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_Light(this);
		}

		return p;
	}

	void PS_LightSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Light * data = (PS_Light *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_LightSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Light * data = (PS_Light *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

	void PS_LightSet::SetLightType(int type)
	{
		mLightType = type;
	}

	void PS_LightSet::SetAttenParam(float start, float end, float fallOff)
	{
		mAttenStart = start;
		mAttenEnd = end;
		mAttenFallOff = fallOff;
	}

	void PS_LightSet::SetSpotParam(float inner, float outer, float fallOff)
	{
		mSpotInner = inner;
		mSpotOuter = outer;
		mSpotFallOff = fallOff;
	}

}