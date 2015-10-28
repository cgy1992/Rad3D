/*
*	PS_EmitterSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_EmitterSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		int mSetIndex;
		int mEmiterIndex;

	public:
		PS_EmitterSet();
		virtual ~PS_EmitterSet();

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Update(float elapsedTime);

		void
			SetSetIndex(int index);
		int
			GetSetIndex() const { return mSetIndex; }

		void
			SetEmitterIndex(int index);
		int
			GetEmitterIndex() const { return mEmiterIndex; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();
	};

}