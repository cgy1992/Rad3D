/*
*	PSSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStack.h"
#include "MRenderQueue.h"
#include "PSEmitter.h"
#include "PSModifier.h"
#include "PSShader.h"

namespace Rad {

	class ParticleSystem;

	class M_ENTRY PS_Set : public IObject
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(IObject);

	public:
		int	mQuota;
		bool mLocalSpace;
		bool mScaleAble;
		bool mEnable;
		bool mSorting;
		
		String OnParticleBorn;
		String OnParticleDie;
		String OnParticleUpdate;
		String OnParticleCollide;

	public:
		PS_Set();
		virtual ~PS_Set();

		virtual void
			OnPropertyChanged(const Property * p);

		ParticleSystem * 
			GetParent() { return mParent; }

		virtual void
			Init(ParticleSystem * parent);
		virtual void
			Update(float elapsedTime);

		void
			Reset();
		int
			_doEmit(PS_Emitter * emitter, float elapsedTime);
		void
			_doModify(PS_Modifier * modifier, float elapsedTime);
		void
			_doEmitModify(PS_Emitter * emitter, float elapsedTime);

		const Aabb &
			GetBound() { return mBound; }

		void
			SetEnable(bool enable);
		bool
			IsEnable() { return mEnable; }

		void		
			SetQuota(int quota);
		int			
			GetQuota() { return mQuota; }

		void		
			SetLocalSpace(bool b);
		bool		
			IsLocalSpace() { return mLocalSpace; }

		void
			SetScaleAble(bool b);
		bool
			IsScaleAble() { return mScaleAble; }

		void 
			SetSorting(bool b);
		bool 
			GetSorting() const { return mSorting; }
		
		PS_Emitter * 
			NewEmitter(const char * type);
		PS_Emitter *
			CloneEmitter(PS_Emitter * emitter);
		void 
			DeleteEmitter(int index);
		int 
			GetEmitterCount() { return mEmitters.Size(); }
		PS_Emitter * 
			GetEmitter(int index) { return mEmitters[index]; }

		PS_Modifier * 
			NewModifier(const char * type);
		PS_Modifier *
			CloneModifier(PS_Modifier * modifier);
		void 
			DeleteModifier(int index);
		int 
			GetModifierCount() { return mModifiers.Size(); }
		PS_Modifier * 
			GetModifier(int index) { return mModifiers[index]; }
		
		PS_ShaderPtr
			CloneShader(PS_ShaderPtr shader);
		void
			SetShader(PS_ShaderPtr shader) { mShader = shader; }
		PS_ShaderPtr
			GetShader() const { return mShader; }
		ShaderFX *
			_getShaderFX() const { return mShader != NULL ? mShader->GetShaderFX() : NULL; }

		int 
			GetParticleCount() const { return mParticles.Size(); }
		const Particle * 
			GetParticle(int index) const { return mParticles[index]; }

		virtual void 
			AllocQuota(int quota) = 0;
		virtual void 
			AddRenderQueue(RenderQueue * rq) = 0;

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

	protected:
		ParticleSystem * mParent;

		Array<Particle> mParticlePool;
		Array<Particle*, t_alloc_pool<Particle*> > mParticleFreeStack;

		Array<PS_Emitter*, t_alloc_pool<PS_Emitter*> > mEmitters;
		Array<PS_Modifier*, t_alloc_pool<PS_Modifier*> > mModifiers;
		Array<Particle*, t_alloc_pool<Particle*> > mParticles;

		PS_ShaderPtr mShader;

		Aabb mBound;
	};

}