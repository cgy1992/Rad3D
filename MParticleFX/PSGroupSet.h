/*
*	PS_GroupSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSGroup.h"

namespace Rad {

	class FX_ENTRY PS_GroupSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		Int4 mSetIndex;

	public:
		PS_GroupSet();
		virtual ~PS_GroupSet();

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Update(float elapsedTime);

		void
			SetSetIndex(const Int4 & sets);
		const Int4
			GetSetIndex() { return mSetIndex; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();
	};

}