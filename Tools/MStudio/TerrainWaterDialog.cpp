#include "stdafx.h"

#include "TerrainWaterDialog.h"

TerrainWaterDialog * TerrainWaterDialog::msInstance = NULL;

TerrainWaterDialog::TerrainWaterDialog()
{
	msInstance = this;

	const MGUI_LookFeel * lk = MGUI_LookFeelManager::Instance()->GetLookFeel(MGUI_WidgetType::Widget, "AllWhite");

	mDialog = new MGUI_Layout(lk, NULL);
	mDialog->SetOrder(MGUI_Order::Top);
	mDialog->SetColor(MGUI_Color(0.15f, 0.15f, 0.15f, 0.5f));
	mDialog->SetAlign(MGUI_Align::Stretch);
	mDialog->SetDrawEnable(false);
	mDialog->SetVisible(false);

	mLayout = MGUI_LayoutLoader::Load("TerrainWater.layout", mDialog);
	mLayout->SetAlign(MGUI_Align::Center);
	mLayout->SetInhertColor(false);
	mLayout->SetInhertAlpha(false);

	mOK = (MGUI_Button *)mLayout->GetChild("OK");
	mCancel = (MGUI_Button *)mLayout->GetChild("Close");
	mOK->SetCaption(L"OK");

	mOK->eventMouseClick += new cListener1<TerrainWaterDialog, const MGUI_MouseEvent *>(this, &TerrainWaterDialog::OnOK);
	mCancel->eventMouseClick += new cListener1<TerrainWaterDialog, const MGUI_MouseEvent *>(this, &TerrainWaterDialog::OnCancel);

	mCaption = new MGUI_TextBox(mLayout->GetChild("Caption"));
	mCaption->SetAlign(MGUI_Align::Center);
	mCaption->SetCaption(L"Terrain");

	_setupTextBox("Name", "T_Name");
	_setupTextBox("GridSize", "T_GridSize");
	_setupTextBox("GridCount", "T_GridCount");
	_setupTextBox("WaveTexture", "T_WaveTexture");
	_setupTextBox("NumKeyFrames", "T_NumKeyFrames");
	_setupTextBox("WaveTime", "T_WaveTime");
	_setupTextBox("UVScale", "T_UVScale");
	_setupTextBox("BaseAlpha", "T_BaseAlpha");
	_setupTextBox("Position", "T_Position");

	mEd_Name = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("Name"));
	mEd_GridSize = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("GridSize"));
	mEd_xGridCount = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("xGridCount"));
	mEd_zGridCount = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("zGridCount"));
	mEd_WaveTexture = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("WaveTexture"));
	mEd_NumKeyFrames = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("NumKeyFrames"));
	mEd_WaveTime = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("WaveTime"));
	mEd_UVScale = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("UVScale"));
	mEd_BaseAlpha = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("BaseAlpha"));

	mEd_PositionX = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("PositionX"));
	mEd_PositionY = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("PositionY"));
	mEd_PositionZ = RTTI_StaticCast(MGUI_EditBox, mLayout->GetChild("PositionZ"));
}

TerrainWaterDialog::~TerrainWaterDialog()
{
	delete mDialog;

	msInstance = NULL;
}

void TerrainWaterDialog::_setupTextBox(const TString128 & caption, const TString128 & parentName)
{
	MGUI_Widget * parentWidget = mLayout->GetChild(parentName);
	d_assert (parentWidget != NULL);
	
	MGUI_TextBox * textBox = new MGUI_TextBox(parentWidget);
	textBox->SetAlign(MGUI_Align::Left | MGUI_Align::VCenter);
	textBox->SetCaption(caption.c_wstr());
}

void TerrainWaterDialog::DoModal(Water * w, void * userData)
{
	Water::Config cfg;

	if (w != NULL)
		cfg = w->GetConfig();

	mUserData = userData;

	TString128 t_Name, t_GridSize, t_xGridCount, t_zGridCount,
		t_WaveTexture, t_NumKeyFrames, t_WaveTime, t_UVScale, t_BaseAlpha;

	TString128 t_PositionX = "0", t_PositionY = "0", t_PositionZ = "0";

	t_Name = cfg.Name;

	t_GridSize.Format("%.2f", cfg.GridSize);
	t_xGridCount.Format("%d", cfg.xGridCount);
	t_zGridCount.Format("%d", cfg.zGridCount);

	t_WaveTexture = cfg.WaveTexture;
	t_NumKeyFrames.Format("%d", cfg.NumKeyframes);
	t_WaveTime.Format("%.2f", cfg.WaveTime);
	t_UVScale.Format("%.2f", cfg.UVScale);
	t_BaseAlpha.Format("%.2f", cfg.BaseAlpha);

	if (w != NULL)
	{
		t_PositionX.Format("%.2f", w->GetPosition().x);
		t_PositionY.Format("%.2f", w->GetPosition().y);
		t_PositionZ.Format("%.2f", w->GetPosition().z);
	}

	mEd_Name->SetCaption(t_Name.c_wstr());
	mEd_GridSize->SetCaption(t_GridSize.c_wstr());
	mEd_xGridCount->SetCaption(t_xGridCount.c_wstr());
	mEd_zGridCount->SetCaption(t_zGridCount.c_wstr());
	mEd_WaveTexture->SetCaption(t_WaveTexture.c_wstr());
	mEd_NumKeyFrames->SetCaption(t_NumKeyFrames.c_wstr());
	mEd_WaveTime->SetCaption(t_WaveTime.c_wstr());
	mEd_UVScale->SetCaption(t_UVScale.c_wstr());
	mEd_BaseAlpha->SetCaption(t_BaseAlpha.c_wstr());

	mEd_PositionX->SetCaption(t_PositionX.c_wstr());
	mEd_PositionY->SetCaption(t_PositionY.c_wstr());
	mEd_PositionZ->SetCaption(t_PositionZ.c_wstr());

	if (w != NULL)
	{
		mEd_GridSize->SetEnable(false);
		mEd_xGridCount->SetEnable(false);
		mEd_zGridCount->SetEnable(false);
	}
	else
	{
		mEd_GridSize->SetEnable(true);
		mEd_xGridCount->SetEnable(true);
		mEd_zGridCount->SetEnable(true);
	}

	mDialog->SetVisible(true);
}

void TerrainWaterDialog::GetConfig(Water::Config & cfg)
{
	TString128 t_Name, t_GridSize, t_xGridCount, t_zGridCount,
		t_WaveTexture, t_NumKeyFrames, t_WaveTime, t_UVScale, t_BaseAlpha;

	t_Name = TString128::FromUnicode(mEd_Name->GetCaption().c_str());
	t_GridSize = TString128::FromUnicode(mEd_GridSize->GetCaption().c_str());
	t_xGridCount = TString128::FromUnicode(mEd_xGridCount->GetCaption().c_str());
	t_zGridCount = TString128::FromUnicode(mEd_zGridCount->GetCaption().c_str());
	t_WaveTexture = TString128::FromUnicode(mEd_WaveTexture->GetCaption().c_str());
	t_NumKeyFrames = TString128::FromUnicode(mEd_NumKeyFrames->GetCaption().c_str());
	t_WaveTime = TString128::FromUnicode(mEd_WaveTime->GetCaption().c_str());
	t_UVScale = TString128::FromUnicode(mEd_UVScale->GetCaption().c_str());
	t_BaseAlpha = TString128::FromUnicode(mEd_BaseAlpha->GetCaption().c_str());

	cfg.Name = t_Name;
	
	cfg.GridSize = t_GridSize.ToFloat();
	cfg.xGridCount = t_xGridCount.ToInt();
	cfg.zGridCount = t_zGridCount.ToInt();

	cfg.WaveTexture = t_WaveTexture;
	cfg.NumKeyframes = t_NumKeyFrames.ToInt();
	cfg.WaveTime = t_WaveTime.ToFloat();
	cfg.UVScale = t_UVScale.ToFloat();

	cfg.BaseAlpha = t_BaseAlpha.ToFloat();
}

Float3 TerrainWaterDialog::GetPosition()
{
	TString128 t_x = TString128::FromUnicode(mEd_PositionX->GetCaption().c_str());
	TString128 t_y = TString128::FromUnicode(mEd_PositionY->GetCaption().c_str());
	TString128 t_z = TString128::FromUnicode(mEd_PositionZ->GetCaption().c_str());

	Float3 Position;

	Position.x = t_x.ToFloat();
	Position.y = t_y.ToFloat();
	Position.z = t_z.ToFloat();

	return Position;
}

void * TerrainWaterDialog::GetUserData()
{
	return mUserData;
}

void TerrainWaterDialog::OnOK(const MGUI_MouseEvent *)
{
	eventEndDialog(true);
	mDialog->SetVisible(false);
}

void TerrainWaterDialog::OnCancel(const MGUI_MouseEvent *)
{
	eventEndDialog(false);
	mDialog->SetVisible(false);
}