/*
*	PS_RibbonSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSRibbon.h"
#include "PSShader.h"

namespace Rad {

	class FX_ENTRY PS_RibbonSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		FixedString32 mShaderClass;
		String mTextureName;
		int mBlendMode;
		bool mKeepAspect;
		int mSegment;
		float mSegmentLife;
		bool mLoose;

	public:
		PS_RibbonSet();
		virtual ~PS_RibbonSet();

		virtual void 
			OnPropertyChanged(const Property * p);

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void
			Init(ParticleSystem * parent);
		virtual void
			Update(float elapsedTime);

		void 
			SetShaderClass(const FixedString32 & name);
		const FixedString32 & 
			GetShaderClass() const { return mShaderClass; }
		
		void 
			SetTexture(const String & name);
		const String & 
			GetTexture() const { return mTextureName; }
		TexturePtr
			_getTexture() { return mTexture; }

		void 
			SetBlendMode(int blendMode);
		int 
			GetBlendMode() { return mBlendMode; }

		void
			SetKeepAspect(bool b);
		bool
			IsKeepAspect() { return mKeepAspect; }

		void
			SetSegment(int segment);
		int
			GetSegment() const { return mSegment; }

		void
			SetSegmentLife(float life);
		float
			GetSegmentLife() const { return mSegmentLife; }

		void
			SetLoose(bool loose);
		bool
			IsLoose() const { return mLoose; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

	protected:
		TexturePtr mTexture;
		PS_Ribbon_RenderObject mRenderObject;
	};
}