#include "PSParticleSystemSet.h"

namespace Rad {

	ImplementRTTI(PS_ParticleSystemSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_ParticleSystemSet)
		DF_PROPERTY_EX(PS_ParticleSystemSet, mPSName, "ParticleSystem", "Filename", "PT_Filename particle(*.particle)|*.particle|", PT_String)
	DF_PROPERTY_END()

	PS_ParticleSystemSet::PS_ParticleSystemSet()
	{
	}

	PS_ParticleSystemSet::~PS_ParticleSystemSet()
	{
		_clear_particle();
	}

	void PS_ParticleSystemSet::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mPSName")
		{
			SetParticleSystem(mPSName);
		}
		else
		{
			PS_Set::OnPropertyChanged(p);
		}
	}

	void PS_ParticleSystemSet::AllocQuota(int quota)
	{
	}

	void PS_ParticleSystemSet::AddRenderQueue(RenderQueue * rq)
	{
		if (mSorting && mParticles.Size())
		{
			Camera * pCamera = World::Instance()->GetCurrentRenderContext()->GetCamera();

			PS_SortOp op;
			op.mRender = this;
			op.mCameraPosition = pCamera->GetWorldPosition();

			Sort(&mParticles[0], mParticles.Size(), op);
		}
	}

	void PS_ParticleSystemSet::Update(float elapsedTime)
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
					PS_ParticleSystem * data = (PS_ParticleSystem *)p->InternalData;

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

	void PS_ParticleSystemSet::SetParticleSystem(const String & name)
	{
		mPSName = name;

		if (mPSName != "")
		{
			mSource = PS_Manager::Instance()->LoadSource(mPSName);
		}
		else
		{
			mSource = NULL;
		}
	}

	Particle * PS_ParticleSystemSet::_quest_particle()
	{
		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			p->InternalData = new PS_ParticleSystem(this);
		}

		return p;
	}

	void PS_ParticleSystemSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_ParticleSystem * data = (PS_ParticleSystem *)p->InternalData;

			delete data;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_ParticleSystemSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_ParticleSystem * data = (PS_ParticleSystem *)p->InternalData;

				delete data;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}