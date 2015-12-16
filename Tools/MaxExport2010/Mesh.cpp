#include "stdafx.h"
#include "Mesh.h"
#include "MMeshSerializer.h"

namespace Exporter {

	TriMesh::TriMesh(int vertexElem)
	{
		mVertexElems = vertexElem;
	}

	TriMesh::~TriMesh()
	{
	}

	bool TriMesh::AddFace(Vertex * face, int vertexElems)
	{
		if (mVertexElems != vertexElems)
			return false;

		if (vertexElems & MeshSerializer::VE_BONEINDEX)
			return _addSkinedFace(face);
		else
			return _addNoSkinedFace(face);
	}

	bool TriMesh::_addNoSkinedFace(Vertex * face)
	{
		unsigned short index[3] = { 0, 0, 0 };

		for (int i = 0; i < 3; ++i)
		{
			Vertex v = face[i];

			index[i] = mVertexList.Add(v);
		}

		mIndexList.PushBack(index[1]);
		mIndexList.PushBack(index[0]);
		mIndexList.PushBack(index[2]);

		return true;
	}

	bool TriMesh::_addSkinedFace(Vertex * face)
	{
		if (!_testFace(face))
			return false;

		unsigned short index[3] = { 0, 0, 0 };

		for (int i = 0; i < 3; ++i)
		{
			Vertex v = face[i];

			BlendIndex bi = v.mBlendIndex;
			for (int b = 0; b < 4; ++b)
			{
				bi.i[b] = _getBoneIndex(bi.i[b]);
			}

			v.mBlendIndex = bi;

			index[i] = mVertexList.Add(v);
		}

		mIndexList.PushBack(index[1]);
		mIndexList.PushBack(index[0]);
		mIndexList.PushBack(index[2]);

		return true;
	}

	bool TriMesh::_testFace(Vertex * face)
	{
		int numNewBones = 0;

		const unsigned char * bindex = NULL;

		for (int i = 0; i < 3; ++i)
		{
			BlendIndex bi = face[i].mBlendIndex;

			if (!_hasBone(bi.i[0]))
				++numNewBones;
			if (!_hasBone(bi.i[1]))
				++numNewBones;
			if (!_hasBone(bi.i[2]))
				++numNewBones;
			if (!_hasBone(bi.i[3]))
				++numNewBones;
		}

		return mBoneIdMap.Size() + numNewBones <= MAX_HW_BONE;
	}

	bool TriMesh::_hasBone(short boneId)
	{
		for (int i = 0; i < mBoneIdMap.Size(); ++i)
		{
			if (mBoneIdMap[i] == boneId)
				return true;
		}

		return false;
	}

	int TriMesh::_getBoneIndex(short boneId)
	{
		int i = 0;

		for (i = 0; i < mBoneIdMap.Size(); ++i)
		{
			if (mBoneIdMap[i] == boneId)
				return i;
		}

		mBoneIdMap.PushBack(boneId);

		return i;
	}

}