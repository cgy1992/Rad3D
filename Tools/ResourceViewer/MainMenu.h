/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

#include "SearchDlg.h"

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	void Layout();

	void Update();

protected:
	void OnOpen(const MGUI::ClickEvent * e);
	void OnSearch(const MGUI::ClickEvent * e);
	void OnNext(const MGUI::ClickEvent * e);
	void OnPrev(const MGUI::ClickEvent * e);
	void OnKeydown(const MGUI::KeyEvent * e);

protected:
	MGUI::Layout * mLayout;
	SearchDlg * mSearchDlg;
};