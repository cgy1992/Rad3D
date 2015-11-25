/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class MSTUDIO_ENTRY ProgressDialog : public Singleton<ProgressDialog>
{
public:
	ProgressDialog();
	~ProgressDialog();

	void 
		Show(bool _show);

	void 
		SetText(const String & text);
	void 
		SetPercentage(int percentage);
	int 
		GetPercentage();

protected:
	MGUI::Layout * mLayout;
	MGUI::Label * mText;
	MGUI::ProgressBarH * mProgressBar;
};