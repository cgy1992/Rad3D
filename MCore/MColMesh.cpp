#include "MColMesh.h"

namespace Rad {

	ColMesh::ColMesh()
		: mVertices(NULL)
		, mIndices(NULL)
		, mNumOfVertices(0)
		, mNumOfIndices(0)
	{
	}

	ColMesh::~ColMesh()
	{
		Clear();
	}

	void ColMesh::Alloc(int numOfVerts, int numOfIndices)
	{
		d_assert (numOfVerts > 0 && numOfIndices > 0);

		safe_delete_array (mVertices);
		safe_delete_array (mIndices);

		mVertices = new Float3[numOfVerts];
		mIndices = new int[numOfIndices];

		mNumOfVertices = numOfVerts;
		mNumOfIndices = numOfIndices;
	}

	void ColMesh::Clear()
	{
		safe_delete_array (mVertices);
		safe_delete_array (mIndices);

		mNumOfVertices = 0;
		mNumOfIndices = 0;
	}

}