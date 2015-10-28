/*
*	PS_Line
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"

namespace Rad {

	class PS_LineSet;

	class FX_ENTRY PS_Line
	{
		DECLARE_POOL_ALLOC();

	public:
		PS_Line(PS_LineSet * parent);
		~PS_Line();

		void 
			_update(Particle * p);
		void
			_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount);
		const Aabb &
			_getBound() { return mBound; }

	protected:
		PS_LineSet * mParent;

		int mSegment;

		float mWidth;
		float mLength;
		float mInvLength;
		float mSegmentStep;
		Aabb mBound;

		Array<Float3, t_alloc_pool<Float3> > mPoints;
	};

	class FX_ENTRY PS_Line_RenderObject : public RenderObject
	{
		friend class PS_LineSet;

	public:
		PS_Line_RenderObject();
		virtual ~PS_Line_RenderObject();

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
		PS_LineSet * mParent;
	};
}