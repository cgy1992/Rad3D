/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "EditorModule.h"
#include "NavGenerator.h"
#include "NavGizmo.h"

class NavPathModule : public EditorModule, public Singleton<NavPathModule>
{
public:
	NavPathModule();
	virtual ~NavPathModule();

	virtual void Layout();
	virtual void Show();
	virtual void Hide();

protected:
	void OnUpdate();
	void OnRender();
	void OnDBClick(const MGUI::MouseEvent *);
	void OnGizmoChanged(Float3 position);
	void OnEditable(const MGUI::Event * e, bool checked);
	void OnBuild(const MGUI::ClickEvent * e);
	void OnLookError(const MGUI::ClickEvent * e);

	void OnSceneLoaded();
	void OnSceneUnload();

protected:
	MGUI::Layout * mLayout;
	NavGizmo * mGizmo;
	NavGenerator mGenerator;
};