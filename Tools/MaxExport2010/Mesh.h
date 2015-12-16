/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "Vertex.h"

namespace Exporter {

	struct TriMesh
	{
		int mVertexElems;
		VertexList mVertexList;
		Array<unsigned short> mIndexList;
		Array<short> mBoneIdMap;

		TriMesh(int vertexElem);
		~TriMesh();

		bool 
			AddFace(Vertex * face, int vertexElems);

	protected:
		bool 
			_addSkinedFace(Vertex * face);
		bool 
			_addNoSkinedFace(Vertex * face);

		bool 
			_testFace(Vertex * face);
		bool 
			_hasBone(short boneId);
		int 
			_getBoneIndex(short boneId);
	};

	struct MeshData
	{
		int VertexElems;

		Array<Float3> P;			// position
		Array<Float3> N;			// normal
		Array<Float4> C;			// vertex color
		Array<Float2> UV;			// uv
		Array<Float2> LUV;			// light map uv
		Array<BlendWeight> BW;		// blend weight
		Array<BlendIndex> BI;		// blend indices

		void Clear()
		{
			VertexElems = 0;

			P.Clear();
			N.Clear();
			C.Clear();
			UV.Clear();
			LUV.Clear();
			BW.Clear();
			BI.Clear();
		}
	};
}