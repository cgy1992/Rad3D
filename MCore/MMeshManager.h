/*
*	Mesh Manager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MSingleton.h"
#include "MMap.h"
#include "MPlane.h"
#include "MAabb.h"
#include "MSphere.h"
#include "MMeshSource.h"

namespace Rad {

	class M_ENTRY MeshManager : public Singleton<MeshManager>
	{
	public:
		MeshManager();
		~MeshManager();

		MeshSourcePtr 
			GetMesh(const String & meshName);
		MeshSourcePtr 
			NewMesh(const String & meshName);
		MeshSourcePtr 
			LoadMesh(const String & filename, float priority = 0);
		void 
			_delMesh(MeshSource * mesh);

		Mesh *
			NewBox(float width, float height, float depth, const Float3 & offset = Float3::Zero);
		Mesh *
			NewSphere(short iRings, short iSegments, float fRadius, const Float3 & offset = Float3::Zero);
		Mesh * 
			NewCylinder(float radius, float height, int rings, const Float3 & offset = Float3::Zero);
		Mesh * 
			NewPlane(const Plane & plane, float width, float height, int iTileW = 1, int iTileH = 1);

	protected:
		MeshSourcePtr
			_find(Hash2 hash, const String & name);

	protected:
		Map<Hash2, MeshSource *> mMeshSourceMap;
	};

}
