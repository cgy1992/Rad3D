/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "WidgetCreateDlg.h"

class LayoutPanel
{
public:
	LayoutPanel();
	~LayoutPanel();

	void 
		Layout();
	MGUI::Layout * 
		GetLayout() { return mLayout; }

	bool 
		SaveLayout(const String & filename);

	void
		Clear();
	void
		OnWidgetLoaded(MGUI::Widget * w);

protected:
	void 
		OnCreate_(const MGUI::MouseEvent * e);
	void 
		OnDelete_(const MGUI::MouseEvent * e);

	void 
		OnWidgetCreate_(bool _ok);
	void 
		OnWidgetSelChanged_(MGUI::TreeViewItem * item);
	void 
		OnNameChanged_(String _name);

protected:
	MGUI::Layout * mLayout;
	MGUI::TreeView * mTreeView;

	WidgetCreateDlg * mWidgetCreateDlg;
};

