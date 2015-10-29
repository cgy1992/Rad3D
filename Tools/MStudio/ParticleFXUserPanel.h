/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class ParticleFXUserPanel : public Singleton<ParticleFXUserPanel>
{
public:
	ParticleFXUserPanel();
	~ParticleFXUserPanel();

	void Layout();
	void Show();
	void Hide();

	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	MGUI::Layout * mLayout;
};