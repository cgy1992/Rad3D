/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "ComponentCreator.h"
#include "EditorFactory.h"

class ComponentPanel : public Singleton<ComponentPanel>
{
public:
	ComponentPanel();
	~ComponentPanel();

	void Layout();
	void Show();
	void Hide();

	MGUI::Layout * GetLayout() { return mLayout; }

	void OnCreateComponent(ComponentFactory * factory);

protected:
	void OnAdd(const MGUI::ClickEvent * e);
	void OnRemove(const MGUI::ClickEvent * e);

	void OnNodeChanged();
	void OnComponentSelChanged(const MGUI::Event * e, int index);

protected:
	MGUI::Layout * mLayout;
	MGUI::ListBox * mListBox;

	ComponentCreator * mCreator;
};