/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class SearchDlg
{
public:
	SearchDlg();
	~SearchDlg();

	void DoModal();

protected:
	void OnOK(const MGUI::ClickEvent *);
	void OnCancel(const MGUI::ClickEvent *);

protected:
	MGUI::Layout * mDialog;
	MGUI::Layout * mLayout;

	MGUI::EditBox * mEditBoxKey;
};