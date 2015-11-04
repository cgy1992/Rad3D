#include "PSSet.h"
#include "PSManager.h"
#include "MParticleSystem.h"
#include "MWorld.h"
#include "MRoot.h"
#include "RadC.h"

namespace Rad {

	DF_PROPERTY_BEGIN(PS_Set)
		DF_PROPERTY(PS_Set, mEnable, "", "Enable", PT_Bool)
		DF_PROPERTY(PS_Set, mQuota, "", "Quota", PT_Int)
		DF_PROPERTY(PS_Set, mLocalSpace, "", "LocalSpace", PT_Bool)
		DF_PROPERTY(PS_Set, mScaleAble, "", "ScaleAble", PT_Bool)
		DF_PROPERTY(PS_Set, mSorting, "", "Sorting", PT_Bool)

		DF_PROPERTY_EX(PS_Set, OnParticleBorn, "Script", "OnBorn", "PT_LongText", PT_String)
		DF_PROPERTY_EX(PS_Set, OnParticleDie, "Script", "OnDie", "PT_LongText", PT_String)
		DF_PROPERTY_EX(PS_Set, OnParticleUpdate, "Script", "OnUpdate", "PT_LongText", PT_String)
		DF_PROPERTY_EX(PS_Set, OnParticleCollide, "Script", "OnCollide", "PT_LongText", PT_String)
	DF_PROPERTY_END()

	void PS_Set::OnPropertyChanged(const Property * p)
	{
		if (p->name == "mQuota")
		{
			SetQuota(mQuota);
		}
		else if (p->name == "mLocalSpace")
		{
			SetLocalSpace(mLocalSpace);
		}
		else if (p->name == "mScaleable")
		{
			SetScaleAble(mScaleAble);
		}
		else
		{
			IObject::OnPropertyChanged(p);
		}
	}

	ImplementRTTI(PS_Set, IObject);

	PS_Set::PS_Set()
		: mParent(NULL)
		, mQuota(10)
		, mLocalSpace(true)
		, mScaleAble(false)
		, mSorting(false)
		, mShader(NULL)
	{
	}

	PS_Set::~PS_Set()
	{
		for (int i = 0; i < mEmitters.Size(); ++i)
		{
			delete mEmitters[i];
		}

		for (int i = 0; i < mModifiers.Size(); ++i)
		{
			delete mModifiers[i];
		}
	}

	void PS_Set::Init(ParticleSystem * parent)
	{
		mParent = parent;

		SetQuota(mQuota);
	}

	void PS_Set::Update(float elapsedTime)
	{
		// update
		for (int i = 0; i < mParticles.Size(); ++i)
		{
			Particle * p = mParticles[i];

			p->Life -= elapsedTime;

			if (p->Life < 0)
			{
				if (OnParticleDie != "")
				{
					radc_varlist i_args;
					i_args.push((void *)mParent);
					i_args.push((void *)p);

					radc_do(OnParticleDie.c_str(), OnParticleDie.Length(), NULL, &i_args);
				}

				_free_particle(p);
				mParticles.Erase(i--);
			}
			else
			{
				p->Position += p->Direction * p->Speed * elapsedTime;

				if (OnParticleCollide != "")
				{
					if (PS_Manager::d_collide(NULL, p, this))
					{
						radc_varlist i_args;
						i_args.push((void *)mParent);
						i_args.push((void *)p);
						radc_do(OnParticleCollide.c_str(), OnParticleCollide.Length(), NULL, &i_args);
					}
				}

				if (OnParticleUpdate != "")
				{
					radc_varlist i_args;
					i_args.push((void *)mParent);
					i_args.push((void *)p);
					radc_do(OnParticleUpdate.c_str(), OnParticleUpdate.Length(), NULL, &i_args);
				}
			}
		}

		// emit
		for (int i = 0; i < mEmitters.Size(); ++i)
		{
			PS_Emitter * emitter = mEmitters[i];

			if (!emitter->IsEnable())
				continue;

			_doEmit(emitter, elapsedTime);
		}

		// affect
		for (int i = 0; i < mModifiers.Size(); ++i)
		{
			PS_Modifier * modifier = mModifiers[i];

			if (!modifier->IsEnable())
				continue;

			_doModify(modifier, elapsedTime);
		}

		//
		mBound.minimum = mParent->GetWorldPosition();
		mBound.maximum = mParent->GetWorldPosition();

		//
		if (mShader != NULL)
		{
			mShader->Update(elapsedTime);
		}
	}

	void PS_Set::Reset()
	{
		_clear_particle();

		mParticlePool.Resize(mQuota);
		for (int i = 0; i < mQuota; ++i)
		{
			mParticleFreeStack.Push(&mParticlePool[i]);
		}

		for (int i = 0; i < mEmitters.Size(); ++i)
		{
			mEmitters[i]->Reset();
		}
	}

	void PS_Set::_doEmit(PS_Emitter * emitter, float elapsedTime)
	{
		emitter->Emit(elapsedTime);

		for (int i = 0; i < emitter->GetEmitCount(); ++i)
		{
			Particle * p = _quest_particle();

			if (!p) break;

			emitter->InitParticle(p);

			if (!IsLocalSpace())
			{
				if (!IsScaleAble())
				{
					p->Position.TransformQ(mParent->GetWorldRotation());
					p->Direction.TransformQ(mParent->GetWorldRotation());

					p->Position += mParent->GetWorldPosition();
				}
				else
				{
					p->Position.TransformA(mParent->GetWorldTM());
					p->Direction.TransformQ(mParent->GetWorldRotation());
				}
			}

			if (IsScaleAble())
			{
				p->Size *= mParent->GetWorldScale();
			}

			if (OnParticleBorn != "")
			{
				radc_varlist i_args;
				i_args.push((void *)mParent);
				i_args.push((void *)p);
				radc_do(OnParticleBorn.c_str(), OnParticleBorn.Length(), NULL, &i_args);
			}

			mParticles.PushBack(p);
		}
	}

	void PS_Set::_doModify(PS_Modifier * modifier, float elapsedTime)
	{
		for (int j = 0; j < mParticles.Size(); ++j)
		{
			modifier->Modify(mParticles[j], elapsedTime);
		}
	}

	void PS_Set::SetEnable(bool enable)
	{
		mEnable = enable;
	}

	void PS_Set::SetQuota(int quota)
	{
		if (quota > 0)
		{
			_clear_particle();

			mParticlePool.Resize(quota);
			for (int i = 0; i < quota; ++i)
			{
				mParticleFreeStack.Push(&mParticlePool[i]);
			}

			mQuota = quota;

			AllocQuota(quota);

			for (int i = 0; i < mEmitters.Size(); ++i)
			{
				mEmitters[i]->Reset();
			}
		}
	}

	void PS_Set::SetLocalSpace(bool b)
	{
		mLocalSpace = b;
	}

	void PS_Set::SetScaleAble(bool b)
	{
		mScaleAble = b;
	}

	void PS_Set::SetSorting(bool b)
	{
		mSorting = b;
	}

	PS_Emitter * PS_Set::NewEmitter(const char * type)
	{
		if (mEmitters.Size() < mEmitters.Capacity())
		{
			PS_Emitter * emitter = Root::NEW_OBJECT_T<PS_Emitter>(type);
			if (emitter)
			{
				emitter->Init(this);
				mEmitters.PushBack(emitter);
			}

			return emitter;
		}
		
		return NULL;
	}

	void PS_Set::DeleteEmitter(int index)
	{
		delete mEmitters[index];
		mEmitters.Erase(index);
	}

	PS_Modifier * PS_Set::NewModifier(const char * type)
	{
		if (mModifiers.Size() < mModifiers.Capacity())
		{
			PS_Modifier * modifier = Root::NEW_OBJECT_T<PS_Modifier>(type);

			if (modifier)
			{
				modifier->Init(this);
				mModifiers.PushBack(modifier);
			}

			return modifier;
		}

		return NULL;
	}

	void PS_Set::DeleteModifier(int index)
	{
		delete mModifiers[index];
		mModifiers.Erase(index);
	}

	Particle * PS_Set::_quest_particle()
	{
		Particle * p = NULL;

		if (mEnable && !mParticleFreeStack.Empty())
		{
			p = mParticleFreeStack.Top();
			mParticleFreeStack.Pop();

			p->InternalData = NULL;
		}

		return p;
	}

	void PS_Set::_free_particle(Particle * p)
	{
		mParticleFreeStack.Push(p);
	}

	void PS_Set::_clear_particle()
	{
		mParticles.Clear();
		mParticleFreeStack.Clear();
		mParticlePool.Clear();
	}

}