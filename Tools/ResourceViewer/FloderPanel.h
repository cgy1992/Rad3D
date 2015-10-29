/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class FloderPanel : public Singleton<FloderPanel>
{
public:
	FloderPanel();
	~FloderPanel();

	void Layout();

	void SetFloder(const String & floder);

protected:
	void OnSelectChanged(MGUI::TreeViewItem * item);

protected:
	MGUI::Layout * mLayout;
	MGUI::TreeView * mTreeView;
	FilePathArchive * mArchive;
	String mRootFloader;
};