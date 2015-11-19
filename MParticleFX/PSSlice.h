/*
*	PS_Slice
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"

namespace Rad {

	class PS_SliceSet;

	class FX_ENTRY PS_Slice
	{
		DECLARE_POOL_ALLOC();

		struct Elem {
			float life;
			Float3 a;
			Float3 b;
		};

	public:
		PS_Slice(PS_SliceSet * parent);
		~PS_Slice();

		void 
			_update(Particle * p, float frameTime);
		void
			_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount);
		const Aabb &
			_getBound() { return mBound; }

	protected:
		PS_SliceSet * mParent;

		int mSegment;
		float mSegmentLife;

		float mWidth;
		float mLength;
		float mInvLength;
		float mSegmentStep;

		Array<Elem, t_alloc_pool<Elem> > mElems;

		Aabb mBound;
	};

	class FX_ENTRY PS_Slice_RenderObject : public RenderObject
	{
	public:
		PS_Slice_RenderObject();
		virtual ~PS_Slice_RenderObject();

		void
			Init(PS_SliceSet * parent);

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
		PS_SliceSet * mParent;
	};

}