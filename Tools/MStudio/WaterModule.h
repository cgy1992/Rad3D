/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "EditorModule.h"
#include "WaterDialog.h"
#include "PropertyGrid.h"

class WaterModule : public EditorModule
{
public:
	WaterModule();
	virtual ~WaterModule();

	void 
		Layout();
	void 
		Show();
	void 
		Hide();

protected:
	void 
		InitBrush();

	void 
		OnWaterSelChanged(const MGUI::Event * e, int index);

	void 
		OnUpdate();
	void 
		OnRender();

	void 
		OnAdd(const MGUI::ClickEvent * e);
	void 
		OnAdd_OK(bool _ok);
	void 
		OnRefresh(const MGUI::ClickEvent * e);
	void 
		OnEdit(const MGUI::ClickEvent * e);
	void 
		OnEdit_OK(bool _ok);

	void 
		OnSceneLoaded();
	void 
		OnSceneUnload();
	void 
		OnNodeDestroy(Node * n);
	void
		OnSectionLoaded(WorldSection * section);
	void
		OnSectionUnload(WorldSection * section);

protected:
	MGUI::Layout * mLayout;

	MGUI::ListBox * mLbWater;

	MGUI::Button * mBnAdd;
	MGUI::Button * mBnRefresh;
	MGUI::Button * mBnEdit;

	Float3 mBrushPosition;

	RenderOp * mRenderOp_Water;
	RenderOp * mRenderOp_Brush;

	ShaderFX * mShaderFX_WaterGrid;
	ShaderFX * mShaderFX_WaterBrush;

	WaterDialog * mWaterDialog;
	MGUI::Layout * mWaterProperty;
	
	PropertyGrid * mPropertyGridWater;
	PropertyGrid * mPropertyGridShader;
};
