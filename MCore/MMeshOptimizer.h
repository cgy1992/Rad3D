/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMeshSource.h"

namespace Rad {

#define MESH_OPT_VERTEX 1
#define MESH_OPT_SKIN 2

	class M_ENTRY MeshOptimizer
	{
	public:
		static void Optimize(MeshSourcePtr mesh, int flags);

	protected:
		static void optimizeVertex(MeshSourcePtr mesh);
		static void optimizeSkin(MeshSourcePtr mesh);
	};

}



