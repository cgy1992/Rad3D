#ifndef TERRAIN_WATER_DIALOG_H
#define TERRAIN_WATER_DIALOG_H

class TerrainWaterDialog
{
	DECLARE_SINGLETON (TerrainWaterDialog);

public:
	tEvent1<bool> eventEndDialog;

public:
	TerrainWaterDialog();
	~TerrainWaterDialog();

	void DoModal(Water * w, void * userData);

	void GetConfig(Water::Config & cfg);
	Float3 GetPosition();
	void * GetUserData();

protected:
	void _setupTextBox(const TString128 & caption, const TString128 & parentName);

protected:
	void OnOK(const MGUI_MouseEvent *);
	void OnCancel(const MGUI_MouseEvent *);

protected:
	MGUI_Layout * mDialog;
	MGUI_Layout * mLayout;

	void * mUserData;

	MGUI_Button * mOK;
	MGUI_Button * mCancel;

	MGUI_TextBox * mCaption;

	MGUI_EditBox * mEd_Name;
	MGUI_EditBox * mEd_GridSize;
	MGUI_EditBox * mEd_xGridCount;
	MGUI_EditBox * mEd_zGridCount;
	MGUI_EditBox * mEd_WaveTexture;
	MGUI_EditBox * mEd_NumKeyFrames;
	MGUI_EditBox * mEd_WaveTime;
	MGUI_EditBox * mEd_UVScale;
	MGUI_EditBox * mEd_BaseAlpha;

	MGUI_EditBox * mEd_PositionX;
	MGUI_EditBox * mEd_PositionY;
	MGUI_EditBox * mEd_PositionZ;
};

#endif