/*
*	PSDecalSet
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "PSDecal.h"
#include "PSShader.h"

namespace Rad {

	class FX_ENTRY PS_DecalSet : public PS_Set
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Set);

	protected:
		FixedString32 mShaderClass;
		String mTextureName;
		int mBlendMode;
		bool mKeepAspect;
		Int2 mGrids;
		Float2 mCenter;

	public:
		PS_DecalSet();
		virtual ~PS_DecalSet();

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
			SetBlendMode(int blendMode);
		int 
			GetBlendMode() { return mBlendMode; }

		void
			SetKeepAspect(bool b);
		bool
			IsKeepAspect() { return mKeepAspect; }

		void
			SetGrids(const Int2 & grids);
		const Int2 &
			GetGrids() { return mGrids; }

		void
			SetCenter(const Float2 & center);
		const Float2 &
			GetCenter() { return mCenter; }

	protected:
		virtual Particle *	
			_quest_particle();
		virtual void		
			_free_particle(Particle * p);
		virtual void 
			_clear_particle();

	protected:
		TexturePtr mTexture;
		PS_Decal_RenderObject mRenderObject;
	};

}