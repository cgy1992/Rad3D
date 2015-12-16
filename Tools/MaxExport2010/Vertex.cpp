#include "stdafx.h"
#include "Mesh.h"

namespace Exporter {

	VertexList::VertexList()
	{
	}

	VertexList::~VertexList()
	{
	}

	int VertexList::Add(const Vertex & v)
	{
		int i = 0;

		for (i = 0; i < Size(); ++i)
		{
			if (mVerts[i] == v)
				return i;
		}

		mVerts.PushBack(v);

		return i;
	}

	int VertexList::Size() const
	{
		return mVerts.Size();
	}

	const Vertex & VertexList::GetVertex(int index) const
	{
		d_assert (index < Size());

		return mVerts[index];
	}

	Vertex & VertexList::GetVertex(int index)
	{
		d_assert (index < Size());

		return mVerts[index];
	}

}
