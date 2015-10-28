/*
*	PS_Ribbon
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"

namespace Rad {

	class PS_RibbonSet;

	class FX_ENTRY PS_Ribbon
	{
		DECLARE_POOL_ALLOC();

		struct Elem {
			float life;
			Float3 position;
		};

	public:
		PS_Ribbon(PS_RibbonSet * parent);
		~PS_Ribbon();

		void 
			_update(Particle * p, float frameTime);
		void
			_updateBuffer(Particle * p, void ** vertexBuffer, void ** indexBuffer, int & startVertex, int & primCount);

	protected:
		PS_RibbonSet * mParent;
		int mSegment;
		float mSegmentLife;

		float mWidth;
		float mLength;
		float mInvLength;
		float mSegmentStep;

		Array<Elem, t_alloc_pool<Elem> > mElems;
	};

	class FX_ENTRY PS_Ribbon_RenderObject : public RenderObject
	{
		friend class PS_RibbonSet;

	public:
		PS_Ribbon_RenderObject();
		virtual ~PS_Ribbon_RenderObject();

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
		PS_RibbonSet * mParent;
	};

}