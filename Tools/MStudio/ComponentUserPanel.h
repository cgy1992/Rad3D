/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class ComponentUserPanel : public Singleton<ComponentUserPanel>
{
public:
	ComponentUserPanel();
	~ComponentUserPanel();

	void Layout();
	void Show();
	void Hide();

	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	MGUI::Layout * mLayout;
};