#ifndef TERRAIN_WATER_PANEL_H
#define TERRAIN_WATER_PANEL_H

#include "TerrainWaterDialog.h"

class TerrainWaterPanel
{
	DECLARE_SINGLETON (TerrainWaterPanel);

public:
	TerrainWaterPanel();
	~TerrainWaterPanel();

	void Layout();
	void Show();
	void Hide();

protected:
	void InitBrush();
	void OnWaterSelChanged();

	void OnUpdate();
	void OnRender();

protected:
	void OnBack(const MGUI_MouseEvent *);
	void OnBack_KeyEnd(MGUI_Component *);

	void OnSize(const MGUI_FocusEvent *);

	void OnAdd(const MGUI_MouseEvent *);
	void OnRemove(const MGUI_MouseEvent *);
	void OnEdit(const MGUI_MouseEvent *);

	void OnRemove_EndDialog(bool _ok);

	void OnWaterDialog_EndDialog(bool _ok);

	void OnWaterClick(const MGUI_MouseEvent *);

	void OnCustomSerialize(bool _isSave);
	void OnSceneUnload();

protected:
	MGUI_Layout * mLayout;

	MGUI_Widget * mTopWidget;
	MGUI_Button * mBnBack;

	MGUI_EditBox * mEdSize;

	MGUI_CheckBox * mCbSet;

	MGUI_Grid * mGrid_Water;

	MGUI_Button * mBnAdd;
	MGUI_Button * mBnRemove;
	MGUI_Button * mBnEdit;

	TerrainWaterDialog * mTerrainWaterDlg;

	MGUI_ImageBox * mImageBox_Selected;

	int mBrushSize;
	Float3 mBrushPosition;

	RenderOp * mRenderOp_Water;
	RenderOp * mRenderOp_Brush;

	ShaderFX * mShaderFX_WaterGrid;
	ShaderFX * mShaderFX_WaterBrush;
};

#endif