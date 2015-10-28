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

		virtual void
			Init(ParticleSystem * parent);

		ParticleSystem * 
			GetParent() { return mParent; }

		virtual void
			Update(float elapsedTime);

		void
			_doEmit(PS_Emitter * emitter, float elapsedTime);
		void
			_doModify(PS_Modifier * modifier, float elapsedTime);

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
		void 
			DeleteEmitter(int index);
		int 
			GetEmitterCount() { return mEmitters.Size(); }
		PS_Emitter * 
			GetEmitter(int index) { return mEmitters[index]; }

		PS_Modifier * 
			NewModifier(const char * type);
		void 
			DeleteModifier(int index);
		int 
			GetModifierCount() { return mModifiers.Size(); }
		PS_Modifier * 
			GetModifier(int index) { return mModifiers[index]; }
		
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
		Stack<Particle*> mParticleFreeStack;

		FixedArray<PS_Emitter *, 8> mEmitters;
		FixedArray<PS_Modifier *, 8> mModifiers;
		Array<Particle*> mParticles;

		PS_ShaderPtr mShader;

		Aabb mBound;
	};

}