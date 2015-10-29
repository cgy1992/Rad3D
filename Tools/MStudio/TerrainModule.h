/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "EditorModule.h"
#include "TerrainHeightPanel.h"
#include "TerrainLayerPanel.h"
#include "TerrainPickerPanel.h"

class TerrainModule : public EditorModule, public Singleton<TerrainModule>
{
public:
	TerrainModule();
	virtual ~TerrainModule();

	virtual void Layout();
	virtual void Show();
	virtual void Hide();

	void _showWithAnim();

protected:
	void OnHeight(const MGUI::MouseEvent *);
	void OnLayer(const MGUI::MouseEvent *);
	void OnPicker(const MGUI::MouseEvent *);

	void OnKeyEnd(MGUI::Component *);

	void OnTerrainCreateDialog_EndDialog(bool _ok);

	void OnSceneUnload();

protected:
	MGUI::Layout * mLayout;

	TerrainHeightPanel * mTerrainHeightPanel;

	TerrainLayerPanel * mTerrainLayerPanel;

	TerrainPickerPanel * mTerrainPickerPanel;
};
