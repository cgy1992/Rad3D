/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyGrid.h"
#include "EditorModule.h"

class EnvironmentModule : public EditorModule, public Singleton<EnvironmentModule>
{
public:
	EnvironmentModule();
	virtual ~EnvironmentModule();

	void Layout();
	void Show();
	void Hide();;

protected:
	void OnSceneAfterLoad();
	void OnPick(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;

	MGUI::Widget * mTopWidget;

	PropertyGrid * mPropertyGrid;
};
