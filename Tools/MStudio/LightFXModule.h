/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "EditorModule.h"
#include "PropertyGrid.h"
#include "LightFX_World.h"
#include "LightFX_UVUnwrap.h"

class LightFXModule : public EditorModule
{
public:
	LightFXModule();
	virtual ~LightFXModule();

	void 
		Layout();
	void 
		Show();
	void 
		Hide();

protected:
	void 
		OnLighting(const MGUI::ClickEvent * e);
	void 
		OnReset(const MGUI::ClickEvent * e);

	void 
		InitWorld();
	void 
		_createPrefab(Prefab * p);
	void 
		_createMesh(Mesh * p, bool onlyCast);
	void 
		_createLight(Light * p);
	void 
		_createTerrain();
	void 
		_createGrassManager();
	bool 
		_checkMesh(Mesh * p);

	void 
		ProcessResult();

	bool 
		EnsureLMapUVAtlas();
	void 
		OnUVAtlas(bool _ok);
	FX_Mesh * 
		UVA_CreateMesh(Mesh * mesh);
	void 
		UVA_SaveMesh(FX_Mesh * fxmesh);
	void
		UVA_UpdateProgress(int progress);

	void 
		OnUpdate();

protected:
	MGUI::Layout * mLayout;
	MGUI::Button * mButton_Lighting;
	MGUI::Button * mButton_Reset;

	PropertyGrid * mPropertyGrid;

	FX_World * mFXWorld;
	bool mDoLighting;

	Set<MeshSource *> mUVAtlasGenMeshSet;
	Array<FX_Mesh *> mUVAMeshes;
	FX_UVUnwrapThread * mUVAThread;
};