/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "EditorModule.h"
#include "ParticleFXPanel.h"
#include "ParticleFXPropertyPanel.h"
#include "ParticleFXUserPanel.h"

class ParticleFXModule : public EditorModule
{
public:
	ParticleFXModule();
	virtual ~ParticleFXModule();

	void Layout();
	void Show();
	void Hide();

protected:
	ParticleFXPanel * mParticleFXPanel;
	ParticleFXPropertyPanel * mPropertyPanel;
	ParticleFXUserPanel * mUserPanel;
};