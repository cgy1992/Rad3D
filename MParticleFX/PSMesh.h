/*
*	PS_Mesh
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "ParticleFX.h"
#include "MMesh.h"

namespace Rad {

	class PS_MeshSet;

	class FX_ENTRY PS_Mesh
	{
		DECLARE_POOL_ALLOC();

	public:
		PS_Mesh(Mesh * mesh, PS_MeshSet * parent);
		virtual ~PS_Mesh();

		void 
			_update(Particle * p);

		const Aabb &
			_getBound() { return mMesh->GetWorldAabb(); }

	protected:
		PS_MeshSet * mParent;
		Mesh * mMesh;
	};

}