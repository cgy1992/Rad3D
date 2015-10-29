/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class TerrainLayerPanel
{
public:
	TerrainLayerPanel();
	~TerrainLayerPanel();

	void Layout();
	void Show();
	void Hide();

protected:
	void OnBack(const MGUI::MouseEvent *);
	void OnBack_KeyEnd(MGUI::Component *);

	void OnSize(const MGUI::FocusEvent *);
	void OnDensity(const MGUI::FocusEvent *);

	void OnProjectLoad(xml_doc & doc);
	void OnProjectUnLoad();

	void OnSceneUnload();
	void OnAfterSceneLoad();

	void OnBrushClick(const MGUI::MouseEvent *);
	void OnLayerClick(const MGUI::MouseEvent *);

	void OnUpdate();
	void _UpdateGeometry();

	void OnRender();

protected:
	MGUI::Layout * mLayout;

	MGUI::EditBox * mEbSize;
	MGUI::EditBox * mEbDensity;

	MGUI::GridBox * mGrid_Brush;
	MGUI::GridBox * mGrid_Layer;

	float mBrushSize;
	float mBrushDensity;
	Float3 mBrushPosition;
	ShaderFX * mShaderFX_Brush;

	MGUI::ImageBox * mImageBox_Brush;
	MGUI::ImageBox * mImageBox_Layer;
};