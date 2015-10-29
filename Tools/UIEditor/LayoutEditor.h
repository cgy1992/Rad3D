/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "LayoutPanel.h"
#include "LayoutProperty.h"
#include "LayoutViewEd.h"

class LayoutEditor : public Singleton<LayoutEditor>
{
public:
	tEvent1<MGUI::Widget *> E_EditWidgetChanged;
	tEvent1<String> E_NameChanged;
	tEvent1<MGUI::Rect> E_RectChanged;

public:
	LayoutEditor();
	~LayoutEditor();

	void 
		Layout();

	void 
		_createLayout(const MGUI::LookFeel * _lk = NULL);
	void 
		_distroyLayout();

	void 
		Open();
	void 
		Save();

	MGUI::Widget *
		GetEditWidget() { return mEditWidget; }
	MGUI::Layout * 
		GetEditLayout() { return mEditLayout; }
	const String & 
		GetLayoutFile() { return mFilename; }

	void 
		notifyEditWidgetChanged(MGUI::Widget * _widget);
	void 
		notifyNameChanged(String _name);
	void 
		notifyRectChanged(MGUI::Rect _rect);

protected:
	void 
		_setLayoutFile(const String & filename);
	void 
		OnEditorModeChanged_(int _mode);

protected:
	LayoutPanel * mLayoutPanel;
	LayoutProperty * mLayoutProperty;
	LayoutViewEd * mLayoutViewEd;

	MGUI::Layout * mEditLayout;
	MGUI::Widget * mEditWidget;

	String mFilename;


};
