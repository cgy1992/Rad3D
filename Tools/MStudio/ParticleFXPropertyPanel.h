/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "PropertyGrid.h"

class ParticleFXPropertyPanel : public Singleton<ParticleFXPropertyPanel>
{
public:
	ParticleFXPropertyPanel();
	~ParticleFXPropertyPanel();

	void Layout();

	void Show();
	void Hide();

	MGUI::Layout * GetLayout() { return mLayout; }

	void AttachSet(PS_Set * pSet);
	void AttachEmitter(PS_Emitter * pEmitter);
	void AttachModifier(PS_Modifier * pModifier);

protected:
	void OnModeChanged(const MGUI::Event * e, int index);
	void OnUpdate();

protected:
	MGUI::Layout * mLayout;
	MGUI::Widget * mTopWidget;

	PropertyGrid * mPropertyGrid_Set;
	PropertyGrid * mPropertyGrid_Emitter;
	PropertyGrid * mPropertyGrid_Modifier;
	PropertyGrid * mPropertyGrid_Shader;
};