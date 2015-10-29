/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class OpenInFloderDlg
{
public:
	OpenInFloderDlg();
	~OpenInFloderDlg();

	void DoModal(float x, float y, const String & filename);

protected:
	void OnClick(const MGUI::ClickEvent * e);
	void OnCancel(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mDialog;
	MGUI::Layout * mLayout;
	MGUI::Button * mButton;

	String mFilename;
};