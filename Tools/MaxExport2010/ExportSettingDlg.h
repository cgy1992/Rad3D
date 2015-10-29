/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

class ExportSettingDlg : public CDialog
{
	DECLARE_MESSAGE_MAP();

public:
	ExportSettingDlg();
	~ExportSettingDlg();

	BOOL 
		OnInitDialog();

	INT_PTR 
		DoModal();
	void 
		OnOK();
};

