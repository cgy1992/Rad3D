/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "PropertyGrid.h"

class ComponentPropertyPanel : public Singleton<ComponentPropertyPanel>
{
public:
	ComponentPropertyPanel();
	~ComponentPropertyPanel();

	void Layout();
	void Show();
	void Hide();

	MGUI::Layout * GetLayout() { return mLayout; }

	void Attach(IObject * p);

protected:
	MGUI::Layout * mLayout;
	MGUI::Widget * mTopWidget;

	PropertyGrid * mPropertyGrid;
};