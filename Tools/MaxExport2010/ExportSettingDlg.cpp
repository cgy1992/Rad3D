#include "stdafx.h"
#include "resource.h"

#include "ExportSettingDlg.h"
#include "ExportConfig.h"

BEGIN_MESSAGE_MAP(ExportSettingDlg, CDialog)
END_MESSAGE_MAP()

ExportSettingDlg::ExportSettingDlg()
	: CDialog(IDD_ExportSetting)
{
}

ExportSettingDlg::~ExportSettingDlg()
{
}

BOOL ExportSettingDlg::OnInitDialog()
{
	if (!CDialog::OnInitDialog())
		return FALSE;

	ExportConfig::Instance()->Load();

	bool expSelected = ExportConfig::Instance()->IsExportSelected();
	bool expNormal = ExportConfig::Instance()->IsExportNormal();
	bool expTexcoord = ExportConfig::Instance()->IsExportTexcoord();
	bool expColor = ExportConfig::Instance()->IsExportColor();
	bool expTangent = ExportConfig::Instance()->IsExportTangent();
	bool expLightmapUV = ExportConfig::Instance()->IsExportLightmapUV();
	bool expAnimation = ExportConfig::Instance()->IsExportAnimation();
	Int2 frameRange = ExportConfig::Instance()->GetFrameRange();

	CButton * bnSelected = (CButton *)GetDlgItem(IDC_CK_Selected);
	CButton * bnNormal = (CButton *)GetDlgItem(IDC_CK_Normal);
	CButton * bnTexcoord = (CButton *)GetDlgItem(IDC_CK_Texcoord);
	CButton * bnColor = (CButton *)GetDlgItem(IDC_CK_Color);
	CButton * bnTangent = (CButton *)GetDlgItem(IDC_CK_Tangent);
	CButton * bnLightmapUV = (CButton *)GetDlgItem(IDC_CK_LightmapUV);
	CButton * bnAnimation = (CButton *)GetDlgItem(IDC_CK_Animation);

	bnSelected->SetCheck(expSelected ? BST_CHECKED : BST_UNCHECKED);
	bnNormal->SetCheck(expNormal ? BST_CHECKED : BST_UNCHECKED);
	bnTexcoord->SetCheck(expTexcoord ? BST_CHECKED : BST_UNCHECKED);
	bnColor->SetCheck(expColor ? BST_CHECKED : BST_UNCHECKED);
	bnTangent->SetCheck(expTangent ? BST_CHECKED : BST_UNCHECKED);
	bnLightmapUV->SetCheck(expLightmapUV ? BST_CHECKED : BST_UNCHECKED);
	bnAnimation->SetCheck(expAnimation ? BST_CHECKED : BST_UNCHECKED);

	SetDlgItemText(IDC_START, i2str(frameRange.x).c_str());
	SetDlgItemText(IDC_END, i2str(frameRange.y).c_str());

	return TRUE;
}

INT_PTR ExportSettingDlg::DoModal()
{
	return CDialog::DoModal();
}

void ExportSettingDlg::OnOK()
{
	CButton * bnSelected = (CButton *)GetDlgItem(IDC_CK_Selected);
	CButton * bnNormal = (CButton *)GetDlgItem(IDC_CK_Normal);
	CButton * bnTexcoord = (CButton *)GetDlgItem(IDC_CK_Texcoord);
	CButton * bnColor = (CButton *)GetDlgItem(IDC_CK_Color);
	CButton * bnTangent = (CButton *)GetDlgItem(IDC_CK_Tangent);
	CButton * bnLightmapUV = (CButton *)GetDlgItem(IDC_CK_LightmapUV);
	CButton * bnAnimation = (CButton *)GetDlgItem(IDC_CK_Animation);

	char startText[32], endText[32];
	GetDlgItemText(IDC_START, startText, 32);
	GetDlgItemText(IDC_END, endText, 32);

	bool expSelected = (bnSelected->GetCheck() == BST_CHECKED);
	bool expNormal = (bnNormal->GetCheck() == BST_CHECKED);
	bool expTexcoord = (bnTexcoord->GetCheck() == BST_CHECKED);
	bool expColor = (bnColor->GetCheck() == BST_CHECKED);
	bool expTangent = (bnTangent->GetCheck() == BST_CHECKED);
	bool expLightmapUV = (bnLightmapUV->GetCheck() == BST_CHECKED);
	bool expAnimation = (bnAnimation->GetCheck() == BST_CHECKED);
	int startFrame = atoi(startText);
	int endFrame = atoi(endText);

	ExportConfig::Instance()->SetExportSelected(expSelected);
	ExportConfig::Instance()->SetExportNormal(expNormal);
	ExportConfig::Instance()->SetExportTexcoord(expTexcoord);
	ExportConfig::Instance()->SetExportColor(expColor);
	ExportConfig::Instance()->SetExportTangent(expTangent);
	ExportConfig::Instance()->SetExportLightmapUV(expLightmapUV);
	ExportConfig::Instance()->SetExportAnimation(expAnimation);
	ExportConfig::Instance()->SetFrameRange(startFrame, endFrame);

	ExportConfig::Instance()->Save();

	CDialog::OnOK();
}
