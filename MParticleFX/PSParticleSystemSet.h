/*
*	PS_PartcileSystemSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSParticleSystem.h"

namespace Rad {

	class FX_ENTRY PS_ParticleSystemSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		String mPSName;

	public:
		PS_ParticleSystemSet();
		virtual ~PS_ParticleSystemSet();

		virtual void 
			OnPropertyChanged(const Property * p);

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Update(float elapsedTime);

		void 
			SetParticleSystem(const String & name);
		const String & 
			GetParticleSystem() const { return mPSName; }
		PS_SourcePtr
			_getSource() { return mSource; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

	protected:
		PS_SourcePtr mSource;
	};

}