/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class MainStatusBar : public Singleton<MainStatusBar>
{
public:
	MainStatusBar();
	~MainStatusBar();

	void Layout();

	void SetText(const String & text);

protected:
	void OnEditorModeChanged_(int mode);

protected:
	MGUI::Layout * mLayout;
	MGUI::Label * mLabel_Text;
};
