#include "PSEmitterSet.h"

namespace Rad {

	ImplementRTTI(PS_EmitterSet, PS_Set);

	DF_PROPERTY_BEGIN(PS_EmitterSet)
		DF_PROPERTY(PS_EmitterSet, mSetIndex, "Emitter", "SetIndex", PT_Int)
		DF_PROPERTY(PS_EmitterSet, mEmiterIndex, "Emitter", "EmiterIndex", PT_Int)
	DF_PROPERTY_END()

	PS_EmitterSet::PS_EmitterSet()
		: mSetIndex(-1)
		, mEmiterIndex(-1)
	{
	}

	PS_EmitterSet::~PS_EmitterSet()
	{
		_clear_particle();
	}

	void PS_EmitterSet::AllocQuota(int quota)
	{
	}

	void PS_EmitterSet::AddRenderQueue(RenderQueue * rq)
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

	void PS_EmitterSet::Update(float elapsedTime)
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
					PS_Emitter * emitter = (PS_Emitter *)p->InternalData;
					if (mParent->IsSetValid(emitter->GetParent()))
					{
						emitter->SetPosition(p->Position);
						emitter->SetRotation(p->Rotation);

						emitter->GetParent()->_doEmit(emitter, elapsedTime);
					}
				}

				Float3 vMin = p->Position - p->Size * 0.5f;
				Float3 vMax = p->Position + p->Size * 0.5f;

				mBound.minimum = Float3::Minimum(mBound.minimum, vMin);
				mBound.maximum = Float3::Maximum(mBound.maximum, vMax);
			}

			if (IsLocalSpace())
			{
				mBound.Transform(mParent->GetWorldTM());
			}
		}
	}

	void PS_EmitterSet::SetSetIndex(int index)
	{
		mSetIndex = index;
	}

	void PS_EmitterSet::SetEmitterIndex(int index)
	{
		mEmiterIndex = index;
	}

	Particle * PS_EmitterSet::_quest_particle()
	{
		if (mSetIndex < 0 || mSetIndex >= mParent->GetSetCount())
			return NULL;

		PS_Set * set = mParent->GetSet(mSetIndex);
		if (set == this)
			return NULL;

		if (mEmiterIndex < 0 || mEmiterIndex >= set->GetEmitterCount())
			return NULL;

		Particle * p = PS_Set::_quest_particle();
		if (p)
		{
			PS_Emitter * emitter = (PS_Emitter *)set->GetEmitter(mEmiterIndex)->Clone();

			emitter->SetEnable(true);

			p->InternalData = emitter;
		}

		return p;
	}

	void PS_EmitterSet::_free_particle(Particle * p)
	{
		if (p->InternalData)
		{
			PS_Emitter * emitter = (PS_Emitter *)p->InternalData;

			delete emitter;

			p->InternalData = NULL;
		}

		PS_Set::_free_particle(p);
	}

	void PS_EmitterSet::_clear_particle()
	{
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			if (p->InternalData)
			{
				PS_Emitter * emitter = (PS_Emitter *)p->InternalData;

				delete emitter;

				p->InternalData = NULL;
			}
		}

		PS_Set::_clear_particle();
	}

}