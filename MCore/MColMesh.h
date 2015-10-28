/*
*	Collision Mesh
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"

namespace Rad {

	enum MColType
	{
		CT_Unknown,
		CT_Mesh,
		CT_Terrain,
	};

	class M_ENTRY ColMesh
	{
		DECLARE_ALLOC();

	public:
		ColMesh();
		~ColMesh();

		void 
			Alloc(int numOfVerts, int numOfIndices);
		void 
			Clear();

		int 
			GetNumOfVertices() { return mNumOfVertices; }
		int 
			GetNumOfIndices() { return mNumOfIndices; }

		Float3 * 
			GetVertices() { return mVertices; }
		int * 
			GetIndices() { return mIndices; }

		bool 
			IsValid() { return mNumOfVertices > 0 && mNumOfIndices > 0; }

	protected:
		int mNumOfVertices;
		Float3 * mVertices;
		
		int mNumOfIndices;
		int * mIndices;

	};

}
