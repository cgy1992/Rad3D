#include "WaterDialog.h"
#include "AllLookFeel.h"

WaterDialog::WaterDialog()
{
	mDialog = new MGUI::Layout(NULL, NULL);
	mDialog->SetOrder(MGUI::eOrder::TOP - 100);
	mDialog->SetColor(Float4(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI::eAlign::STRETCH);
	mDialog->SetVisible(false);

	mLayout = MGUI::Layout::Load("WaterInfo.layout", mDialog);
	mLayout->SetAlign(MGUI::eAlign::CENTER);
	mLayout->SetInheritColor(false);
	mLayout->SetInheritAlpha(false);

	mOK = (MGUI::Button *)mLayout->GetChild("OK");
	mCancel = (MGUI::Button *)mLayout->GetChild("Close");
	mOK->SetCaption(L"OK");

	mOK->E_MouseClick += new cListener1<WaterDialog, const MGUI::MouseEvent *>(this, &WaterDialog::OnOK);
	mCancel->E_MouseClick += new cListener1<WaterDialog, const MGUI::MouseEvent *>(this, &WaterDialog::OnCancel);

	mEd_GridSize = mLayout->GetChildT<MGUI::EditBox>("GridSize");;
	mEd_xGridCount = mLayout->GetChildT<MGUI::EditBox>("xGridCount");
	mEd_zGridCount = mLayout->GetChildT<MGUI::EditBox>("zGridCount");
}

WaterDialog::~WaterDialog()
{
	delete mDialog;
}

void WaterDialog::DoModal(Water * pWater)
{
	float gridSize = UNIT_METRES;
	int xGridCount = 32;
	int zGridCount = 32;

	if (pWater != NULL)
	{
		gridSize = pWater->GetGridSize();
		xGridCount = pWater->GetGridWidth();
		zGridCount = pWater->GetGridHeight();
	}

	String t_GridSize, t_xGridCount, t_zGridCount;

	t_GridSize = f2str(gridSize, true);
	t_xGridCount.Format("%d", xGridCount);
	t_zGridCount.Format("%d", zGridCount);

	mEd_GridSize->SetCaption(t_GridSize.c_wstr());
	mEd_xGridCount->SetCaption(t_xGridCount.c_wstr());
	mEd_zGridCount->SetCaption(t_zGridCount.c_wstr());

	mDialog->SetVisible(true);
}

float WaterDialog::GetGridSize()
{
	String str;

	str.FromUnicode(mEd_GridSize->GetCaption().c_str());

	return str.ToFloat();
}

int WaterDialog::GetXGridCount()
{
	String str;

	str.FromUnicode(mEd_zGridCount->GetCaption().c_str());

	return str.ToInt();
}

int WaterDialog::GetZGridCount()
{
	String str;

	str.FromUnicode(mEd_xGridCount->GetCaption().c_str());

	return str.ToInt();
}

void WaterDialog::OnOK(const MGUI::MouseEvent *)
{
	E_EndDialog(true);
	E_EndDialog.DetachAll();
	mDialog->SetVisible(false);
}

void WaterDialog::OnCancel(const MGUI::MouseEvent *)
{
	E_EndDialog(false);
	E_EndDialog.DetachAll();
	mDialog->SetVisible(false);
}