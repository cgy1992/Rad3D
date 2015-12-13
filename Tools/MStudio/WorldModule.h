/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PropertyGrid.h"
#include "EditorModule.h"

class WorldModule : public EditorModule, public Singleton<WorldModule>
{
public:
	WorldModule();
	virtual ~WorldModule();

	void 
		Layout();
	void 
		Show();
	void 
		Hide();;

protected:
	void 
		OnSceneLoaded();
	void
		OnSceneUnload();

	void
		OnSelectChanged(const MGUI::Event * e, int index);
	void
		OnAdd(const MGUI::ClickEvent * e);
	void
		OnAdd_OK(const MGUI::ClickEvent * e);
	void
		OnAdd_Cancel(const MGUI::ClickEvent * e);
	void
		OnRemove(const MGUI::ClickEvent * e);
	void 
		OnPick(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;

	MGUI::ListBox * mListBox;

	PropertyGrid * mPropertyGrid;

	MGUI::Layout * mSelectLayout;
	MGUI::ComboBox * mSelectType;
	Array<FixedString32> mClassNames;
};
