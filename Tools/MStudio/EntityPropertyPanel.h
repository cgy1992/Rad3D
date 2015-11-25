/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "PropertyGrid.h"

class MSTUDIO_ENTRY EntityPropertyPanel : public Singleton<EntityPropertyPanel>
{
public:
	EntityPropertyPanel();
	~EntityPropertyPanel();

	void 
		Layout();

	void 
		Show();
	void 
		Hide();

	MGUI::Layout * 
		GetLayout() { return mLayout; }

protected:
	void 
		OnNodeSelectedChanged();

protected:
	MGUI::Layout * mLayout;
	MGUI::Widget * mTopWidget;
	
	PropertyGrid * mPropertyGrid;
};