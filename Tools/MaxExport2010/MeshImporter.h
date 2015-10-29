#pragma once

class MeshImporter
{
public:
	MeshImporter();
	~MeshImporter();

	bool DoImport(const char * filename, Interface * gi);

protected:
	bool _impMesh(Interface * gi);

protected:
	Root * mRoot;
	ResourceManager * mResourceManager;
	NullRenderSystem * mRenderSystem;
	World * mWorld;

	MeshSourcePtr mMeshSource;
};