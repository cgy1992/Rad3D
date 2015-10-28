/*
*	MeshGroup
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MMultiMap.h"
#include "MMesh.h"

namespace Rad {

	class M_ENTRY MeshGroup
	{
	public:
		MeshGroup();
		~MeshGroup();

		void
			Append(Mesh * mesh);

		void
			Build();
		void
			Destroy();

	protected:
		void
			_build(int index);
		void
			_genMesh(const Array<Mesh *> & arr, int first, int last, bool hasLightingColor);

	protected:
		MultiMap<MeshSource *, Mesh *> mMeshBucket[2];
		Array<Mesh *> mMeshes;
	};

}