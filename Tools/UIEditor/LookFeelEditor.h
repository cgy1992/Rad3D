/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LookFeelPanel.h"
#include "LookFeelProperty.h"
#include "LookFeelSkinEd.h"

class LookFeelEditor : public Singleton<LookFeelEditor>
{
public:
	tEvent1<MGUI::LookFeel *> E_EditLookFeelChanged;
	tEvent1<Texture *> E_EditSkinChanged;
	tEvent1<MGUI::Rect> E_EditRectChanged;
	tEvent1<MGUI::ClientRect> E_EditClientChanged;
	tEvent0 E_NameChanged;
	tEvent0 E_OpenLookFeel;

public:
	LookFeelEditor();
	~LookFeelEditor();

	void Layout();

	void Open();
	void Save();

	void SetEditLookFeel(MGUI::LookFeel * _lookfeel);
	MGUI::LookFeel * GetEditLookFeel();
	Texture * GetEditSkin();
	const String & GetEditSkinName();

	const String & GetLookFeelFile();

	int GetEditWidgetType();

	void _notifyNameChanged();
	void _notifyRectChanged(const MGUI::Rect & _rect);
	void _notifyClientChanged(const MGUI::ClientRect & _rect);
	void _notifySkinChanged(Texture * _skin);

protected:
	void _setLookFeelFile(const String & filename);

	void OnEditorModeChanged_(int _mode);

protected:
	LookFeelPanel * mLookFeelPanel;
	LookFeelProperty * mLookFeelProperty;
	LookFeelSkinEd * mLookFeelSkinEd;

	MGUI::LookFeel * mEditLookFeel;
	TexturePtr mEditSkin;
	MGUI::Rect mEditRect;

	String mFilename;
};

