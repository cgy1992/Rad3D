/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class TerrainPickerPanel
{
public:
	TerrainPickerPanel();
	~TerrainPickerPanel();

	void Layout();
	void Show();
	void Hide();

protected:
	void OnBack(const MGUI::ClickEvent * e);
	void OnBack_KeyEnd(MGUI::Component * cp);

	void OnRemove(const MGUI::ClickEvent * e);
	void OnUnused(const MGUI::Event * e, bool checked);

	void OnSelectLayer(const MGUI::MouseEvent * e);

	void OnPick(const MGUI::MouseEvent * e);

	void OnSceneUnload();
	void OnSceneLoaded();

	void _updateGeomety(int x, int z);
	void OnRender();

protected:
	MGUI::Layout * mLayout;

	MGUI::CheckBox * mCheckBox_Unused;
	MGUI::GridBox * mGrid_Layer;
	MGUI::ImageBox * mImageBox_Layer;

	ShaderFX * mShaderFX;
	RenderOp * mRenderOp;

	TerrainMesh * mPickedSection;
};