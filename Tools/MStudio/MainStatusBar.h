/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class MainStatusBar
{
public:
	MainStatusBar();
	~MainStatusBar();

	void 
		Layout();

private:
	void 
		OnUpdate(const MGUI::Event *);

protected:
	MGUI::Layout * mLayout;
	MGUI::TextBox * mTextBox;
};

