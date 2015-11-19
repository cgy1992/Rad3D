/*
*	GrassRenderer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRenderObject.h"
#include "MNode.h"
#include "MGrass.h"

namespace Rad {

	class GrassSection;

	class M_ENTRY GrassRenderer : public Node, public RenderObject
	{
		DECLARE_RTTI();

	public:
		GrassRenderer(GrassSection * section);
		virtual ~GrassRenderer();

		virtual void 
			Update(float elapsedTime);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		virtual void 
			_getWorldPosition(Float3 & pos);

	protected:
		GrassSection * mSection;
		Array<const GrassInstance *> mArray;
	};

}