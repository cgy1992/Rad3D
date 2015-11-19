/*
*	PS_LineSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSLine.h"
#include "PSShader.h"

namespace Rad {

	class FX_ENTRY PS_LineSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		FixedString32 mShaderClass;
		String mTextureName;
		int mBlendMode;
		bool mKeepAspect;
		int mSegment;
		float mTileScale;

		float mWaveStep;
		Float3 mWaveDir;
		Float3 mWaveFreq;
		Float3 mWaveWeight;

	public:
		PS_LineSet();
		virtual ~PS_LineSet();

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
			SetTileScale(float scale);
		float
			GetTileScale() const { return mTileScale; }

		void
			SetWaveStep(float step) { mWaveStep = step; }
		float
			GetWaveStep() const { return mWaveStep; }

		void
			SetWaveDir(const Float3 & dir) { mWaveDir = dir; }
		const Float3 &
			GetWaveDir() const { return mWaveDir; }

		void
			SetWaveFreq(const Float3 & freq) { mWaveFreq = freq; }
		const Float3 &
			GetWaveFreq() const { return mWaveFreq; }

		void
			SetWaveWeight(const Float3 & weight) { mWaveWeight = weight; }
		const Float3 &
			GetWaveWeight() const { return mWaveWeight; }

		void
			SetPoint(const Float3 & point1, const Float3 & point2);
		const Float3 &
			GetPoint1() const { return mPoint1; }
		const Float3 &
			GetPoint2() const { return mPoint2; }
		bool
			IsUsePoint() const { return mUsePoint; }
		
	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

	protected:
		TexturePtr mTexture;
		PS_Line_RenderObject mRenderObject;

		bool mUsePoint;
		Float3 mPoint1;
		Float3 mPoint2;
	};
}