/*
*	PSDecal
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"

namespace Rad {

	class PS_DecalSet;

	class FX_ENTRY PS_Decal
	{
		DECLARE_POOL_ALLOC();

	public:
		PS_Decal(PS_DecalSet * parent);
		~PS_Decal();

		void 
			_update(Particle * p);
		void
			_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount);
		const Aabb &
			_getBound() { return mBound; }

	protected:
		PS_DecalSet * mParent;

		int mXGrids, mZGrids;

		float mWidth, mLength;
		float mXStep, mZStep;
		float mInvWidth, mInvLength;

		Array<Float3, t_alloc_pool<Float3> > mPoints; 

		Aabb mBound;
	};

	class FX_ENTRY PS_Decal_RenderObject : public RenderObject
	{
	public:
		PS_Decal_RenderObject();
		virtual ~PS_Decal_RenderObject();

		void
			Init(PS_DecalSet * parent);

		virtual void 
			_getWorldPosition(Float3 & pos);
		virtual void 
			_getWorldBound(Aabb & bound);
		virtual void 
			_getWorldTM(Mat4 & form);

		void
			_allocQuota(int quota);
		void
			_update();

		void *
			_lockVertex();
		void *
			_lockIndex();
		void
			_unlock(int primCount);

	protected:
		PS_DecalSet * mParent;
	};
}