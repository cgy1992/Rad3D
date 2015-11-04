/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "ParticleFXNewDialog.h"

class ParticleFXPanel : public Singleton<ParticleFXPanel>
{
public:
	ParticleFXPanel();
	~ParticleFXPanel();

	void Layout();
	void Show();
	void Hide();

	void Clear();

	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	void OnNodeDestroy(Node * node);
	void OnSceneUnLoad();

	void OnSave(const MGUI::ClickEvent * e);
	void OnReplay(const MGUI::ClickEvent * e);

	void OnSetSelChanged(const MGUI::Event * e, int index);
	void OnEmitterSelChanged(const MGUI::Event * e, int index);
	void OnModifierSelChanged(const MGUI::Event * e, int index);

	void OnAddSet(const MGUI::ClickEvent * e);
	void OnAddSet_End(bool _ok);
	void OnDelSet(const MGUI::ClickEvent * e);
	void OnCopySet(const MGUI::ClickEvent * e);

	void OnAddEmitter(const MGUI::ClickEvent * e);
	void OnAddEmitter_End(bool _ok);
	void OnDelEmitter(const MGUI::ClickEvent * e);

	void OnAddModifier(const MGUI::ClickEvent * e);
	void OnAddModifier_End(bool _ok);
	void OnDelModifier(const MGUI::ClickEvent * e);

protected:
	PS_Set * _getSelectSet();

protected:
	MGUI::Layout * mLayout;

	MGUI::Button * mButton_Save;
	MGUI::Button * mButton_Replay;

	MGUI::ListBox * mListBox_Set;
	MGUI::Button * mButton_NewSet;
	MGUI::Button * mButton_DelSet;
	MGUI::Button * mButton_CopySet;

	MGUI::ListBox * mListBox_Emitter;
	MGUI::Button * mButton_NewEmitter;
	MGUI::Button * mButton_DelEmitter;

	MGUI::ListBox * mListBox_Modifier;
	MGUI::Button * mButton_NewModifier;
	MGUI::Button * mButton_DelModifier;

	ParticleSystem * mParticleSystem;

	ParticleFXNewSetDialog * mParticleNewSetDialog;
	ParticleFXNewEmitterDialog * mParticleNewEmitterDialog;
	ParticleFXNewModifierDialog * mParticleNewModifierDialog;
};