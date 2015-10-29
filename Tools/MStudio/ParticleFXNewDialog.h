/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class ParticleFXNewDialog
{
public:
	tEvent1<bool> E_EndDialog;

public:
	ParticleFXNewDialog();
	~ParticleFXNewDialog();

	void DoModal();

	String GetTypeName();

protected:
	void OnOK(const MGUI::ClickEvent * e);
	void OnCancel(const MGUI::ClickEvent * e);

protected:
	MGUI::Layout * mDialog;
	MGUI::Layout * mLayout;

	MGUI::ComboBox * mComboBox_Type;
	MGUI::Button * mButton_OK;
	MGUI::Button * mButton_Cancel;
};


class ParticleFXNewSetDialog : public ParticleFXNewDialog
{
public:
	ParticleFXNewSetDialog();
	~ParticleFXNewSetDialog();

protected:
	Array<FixedString32> mNames;
};

class ParticleFXNewEmitterDialog : public ParticleFXNewDialog
{
public:
	ParticleFXNewEmitterDialog();
	~ParticleFXNewEmitterDialog();

protected:
	Array<FixedString32> mNames;
};

class ParticleFXNewModifierDialog : public ParticleFXNewDialog
{
public:
	ParticleFXNewModifierDialog();
	~ParticleFXNewModifierDialog();

protected:
	Array<FixedString32> mNames;
};