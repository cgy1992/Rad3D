/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "EditorModule.h"

class GrassModule : public EditorModule
{
public:
	GrassModule();
	virtual ~GrassModule();

	void Layout();
	void Show();
	void Hide();

protected:
	void OnUpdate();
	void OnRender();

protected:
	void OnSize(const MGUI::FocusEvent *);

	void OnGrassDialog_EndDialog(bool _ok);
	void OnRemove_EndDialog(bool _ok);

	void OnGrassClick(const MGUI::MouseEvent *);

	void OnSceenLoaded();
	void OnSceneUnload();

	void OnSectionChanged(WorldSection * section);

protected:
	MGUI::Layout * mLayout;

	MGUI::EditBox * mEdMinSize;
	MGUI::EditBox * mEdMaxSize;

	MGUI::GridBox * mGrid_Grass;

	MGUI::ImageBox * mImageBox_Selected;

	float mBrushSize;
	float mMinSize, mMaxSize;
	Float3 mBrushPosition;
	ShaderFX * mShaderFX_Brush;
	ShaderFX * mShaderFX_TerrainMeshBrush;
};

