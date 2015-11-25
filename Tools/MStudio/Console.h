/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class Console : public Singleton<Console>
{
public:
	Console();
	~Console();

	void 
		SetVisible(bool b);
	bool
		IsVisible();

	void
		Print(const char * fmt, ...);

protected:
	void
		OnClose(const MGUI::ClickEvent * e);
	void
		OnClear(const MGUI::ClickEvent * e);

protected:
	MGUI::Layout * mLayout;
	MGUI::ListBox * mListBox;
};