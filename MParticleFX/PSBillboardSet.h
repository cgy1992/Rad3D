/*
*	PSBillboardSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "PSParticle.h"
#include "PSBillboard.h"
#include "PSShader.h"

namespace Rad {

	class FX_ENTRY PS_BillboardSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		FixedString32 mShaderClass;
		String mTextureName;
		int mBillboardType;
		Float2 mBillboardCenter;
		Float3 mCommonDirection;
		Float3 mCommonUpVector;
		int mBlendMode;
		bool mDepthCheck;
		bool mAccurateFacing;
		bool mKeepAspect;

	public:
		PS_BillboardSet();
		virtual ~PS_BillboardSet();

		virtual void
			OnPropertyChanged(const Property * p);

		virtual void 
			AllocQuota(int quota);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

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
			SetBillboardType(int type);
		int 
			GetBillboardType() const { return mBillboardType; }

		void
			SetBillboardCenter(const Float2 & center);
		const Float2 &
			GetBillboardCenter() const { return mBillboardCenter; }

		void 
			SetCommonDirection(const Float3 & dir);
		const Float3 & 
			GetCommonDirection() const { return mCommonDirection; }

		void 
			SetCommonUpVector(const Float3 & up);
		const Float3 & 
			GetCommonUpVector() const { return mCommonUpVector; }

		void 
			SetBlendMode(int blendMode);
		int 
			GetBlendMode() { return mBlendMode; }

		void
			SetDepthCheck(bool b);
		bool
			IsDepthCheck() { return mDepthCheck; }

		void 
			SetAccurateFacing(bool facing);
		bool 
			IsAccurateFacing() const { return mAccurateFacing; }

		void
			SetKeepAspect(bool b);
		bool
			IsKeepAspect() { return mKeepAspect; }

	protected:
		TexturePtr mTexture;
		PS_Billboard mBillboard;
	};

}