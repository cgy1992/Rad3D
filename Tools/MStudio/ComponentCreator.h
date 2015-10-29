/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class ComponentCreator
{
public:
	ComponentCreator();
	~ComponentCreator();

	void DoModal();

protected:
	void OnOK(const MGUI::ClickEvent *);
	void OnCancel(const MGUI::ClickEvent *);

protected:
	MGUI::Layout * mDialog;
	MGUI::Layout * mLayout;
	MGUI::ListBox * mListBox;
};