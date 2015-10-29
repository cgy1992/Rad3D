/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class WaterDialog
{
public:
	tEvent1<bool> E_EndDialog;

public:
	WaterDialog();
	~WaterDialog();

	void DoModal(Water * pWater);

	float GetGridSize();
	int GetXGridCount();
	int GetZGridCount();

protected:
	void OnOK(const MGUI::MouseEvent *);
	void OnCancel(const MGUI::MouseEvent *);

protected:
	MGUI::Layout * mDialog;
	MGUI::Layout * mLayout;

	void * mUserData;

	MGUI::Button * mOK;
	MGUI::Button * mCancel;

	MGUI::EditBox * mEd_GridSize;
	MGUI::EditBox * mEd_xGridCount;
	MGUI::EditBox * mEd_zGridCount;
};
