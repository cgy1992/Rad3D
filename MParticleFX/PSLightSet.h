/*
*	PS_LightSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSLight.h"

namespace Rad {

	class FX_ENTRY PS_LightSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		int mLightType;

		float mAttenStart;
		float mAttenEnd;
		float mAttenFallOff;

		float mSpotInner;
		float mSpotOuter;
		float mSpotFallOff;

	public:
		PS_LightSet();
		virtual ~PS_LightSet();

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Update(float elapsedTime);

		void
			SetLightType(int type);
		int
			GetLightType() const { return mLightType; }

		void            
			SetAttenParam(float start, float end, float fallOff);
		float			
			GetAttenStart() const { return mAttenStart; }
		float           
			GetAttenEnd() const { return mAttenEnd; }
		float           
			GetAttenFallOff() const { return mAttenFallOff; }
		Float4			
			GetAttenParam() const { return Float4(mAttenStart, mAttenEnd, 1 / (mAttenEnd - mAttenStart), mAttenFallOff);}

		void            
			SetSpotParam(float inner, float outer, float fallOff);
		float          
			GetSpotInner() const { return mSpotInner; }
		float           
			GetSpotOuter() const { return mSpotOuter; }
		float			
			GetSpotFallOff() const { return mSpotFallOff; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();
	};
}