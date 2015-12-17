/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "Mesh.h"
#include "TextureExporter.h"

struct MMPair
{
	IGameMaterial * mtl;
	Exporter::TriMesh * mesh;
};

struct MMSorter
{
	bool operator ()(const MMPair & mm1, const MMPair & mm2)
	{
		return (int)mm1.mtl <= (int)mm2.mtl;
	}
};

class MeshExporter
{
public:
	MeshExporter(IGameScene * mGameScene);
	~MeshExporter();

	void
		Export();

protected:
	void
		ExtractSkeleton(IGameNode * node);
	int 
		_getJointId(const char * name);
	Animation *
		_getAnimation();
	void 
		_dumpJoint(IGameNode * node);
	void
		_dumpAnimation(IGameControl * pGameControl, int boneId);

	void 
		ExtractMesh(IGameNode * node);
	void 
		_dumpSkinInfo(IGameSkin * skin);
	void 
		_genSkinInfo(IGameNode * bone);
	void 
		_dumpMeshBuffer(IGameMesh * mesh);
	void 
		_dumpFace(Exporter::Vertex * f, FaceEx * face);

	void
		BuildMesh();
	void 
		_dumpMaterial(Rad::Material * m, IGameMaterial * mtl);

protected:
	IGameScene * mGameScene;

	Root * mRoot;
	ResourceManager * mResourceManager;
	NullRenderSystem * mRenderSystem;
	World * mWorld;

	MeshSourcePtr mMeshSource;

	Exporter::MeshData mMeshData;
	Array<MMPair> mMMPairs;

	int mStartFrameTicks;
	int mEndFrameTicks;
};
