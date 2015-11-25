#include "ParticleFXPanel.h"
#include "MainFrame.h"
#include "Editor.h"
#include "AllLookFeel.h"
#include "ParticleFXPropertyPanel.h"
#include "MUtil.h"

ImplementSingleton(ParticleFXPanel);

ParticleFXPanel::ParticleFXPanel()
{
	mLayout = MGUI::Layout::Load("ParticleFXPanel.layout", NULL);
	d_assert(mLayout != NULL);

	mButton_Save = (MGUI::Button *)mLayout->GetChild("Save");
	mButton_Replay = mLayout->GetChildT<MGUI::Button>("Replay");

	mListBox_Set = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout->GetChild("Set")->GetChild("List"));
	mListBox_Set->SetAlign(MGUI::eAlign::STRETCH);
	mListBox_Set->SetVScrollBar(AllLookFeel::Instance()->GetVScrollBar());

	mButton_NewSet = (MGUI::Button *)mLayout->GetChild("Set")->GetChild("New");
	mButton_DelSet = (MGUI::Button *)mLayout->GetChild("Set")->GetChild("Delete");
	mButton_CopySet = (MGUI::Button *)mLayout->GetChild("Set")->GetChild("Copy");

	mListBox_Emitter = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout->GetChild("Emitter")->GetChild("List"));
	mListBox_Emitter->SetAlign(MGUI::eAlign::STRETCH);

	mButton_NewEmitter = (MGUI::Button *)mLayout->GetChild("Emitter")->GetChild("New");
	mButton_DelEmitter = (MGUI::Button *)mLayout->GetChild("Emitter")->GetChild("Delete");

	mListBox_Modifier = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout->GetChild("Modifier")->GetChild("List"));
	mListBox_Modifier->SetAlign(MGUI::eAlign::STRETCH);

	mButton_NewModifier = (MGUI::Button *)mLayout->GetChild("Modifier")->GetChild("New");
	mButton_DelModifier = (MGUI::Button *)mLayout->GetChild("Modifier")->GetChild("Delete");

	mButton_Save->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnSave);
	mButton_Replay->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnReplay);

	mListBox_Set->E_SelectChanged += new cListener2<ParticleFXPanel, const MGUI::Event *, int>(this, &ParticleFXPanel::OnSetSelChanged);
	mButton_NewSet->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnAddSet);
	mButton_DelSet->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnDelSet);
	mButton_CopySet->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnCopySet);

	mListBox_Emitter->E_SelectChanged += new cListener2<ParticleFXPanel, const MGUI::Event *, int>(this, &ParticleFXPanel::OnEmitterSelChanged);
	mButton_NewEmitter->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnAddEmitter);
	mButton_DelEmitter->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnDelEmitter);

	mListBox_Modifier->E_SelectChanged += new cListener2<ParticleFXPanel, const MGUI::Event *, int>(this, &ParticleFXPanel::OnModifierSelChanged);
	mButton_NewModifier->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnAddModifier);
	mButton_DelModifier->E_Click += new cListener1<ParticleFXPanel, const MGUI::ClickEvent *>(this, &ParticleFXPanel::OnDelModifier);

	Editor::Instance()->E_NodeDestroy += new cListener1<ParticleFXPanel, Node *>(this, &ParticleFXPanel::OnNodeDestroy);
	Editor::Instance()->E_SceneUnload += new cListener0<ParticleFXPanel>(this, &ParticleFXPanel::OnSceneUnLoad);

	mParticleNewSetDialog = new ParticleFXNewSetDialog;
	mParticleNewEmitterDialog = new ParticleFXNewEmitterDialog;
	mParticleNewModifierDialog = new ParticleFXNewModifierDialog;
}

ParticleFXPanel::~ParticleFXPanel()
{
	delete mParticleNewSetDialog;
	delete mParticleNewEmitterDialog;
	delete mParticleNewModifierDialog;

	Clear();

	safe_delete (mLayout);
}

void ParticleFXPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;

	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void ParticleFXPanel::Show()
{
	mLayout->SetVisible(true);

	Editor::Instance()->ClearFlags(FLAG_ALL);

	Clear();

	Node * node = Editor::Instance()->GetSelectNode();
	if (KIND_OF(ParticleSystem, node))
	{
		mParticleSystem = (ParticleSystem *)node;

		for (int i = 0; i < mParticleSystem->GetSetCount(); ++i)
		{
			PS_Set * pSet = mParticleSystem->GetSet(i);

			String name = pSet->GetRTTI()->Name();

			mListBox_Set->Append(name.c_wstr(), pSet);
		}
	}
}

void ParticleFXPanel::Hide()
{
	mLayout->SetVisible(false);
}

void ParticleFXPanel::Clear()
{
	ParticleFXPropertyPanel::Instance()->AttachSet(NULL);
	ParticleFXPropertyPanel::Instance()->AttachEmitter(NULL);
	ParticleFXPropertyPanel::Instance()->AttachModifier(NULL);

	mListBox_Set->Clear();
	mListBox_Emitter->Clear();
	mListBox_Modifier->Clear();

	mParticleSystem = NULL;
}

void ParticleFXPanel::OnNodeDestroy(Node * node)
{
	Clear();
}

void ParticleFXPanel::OnSceneUnLoad()
{
	Clear();
}

void ParticleFXPanel::OnSave(const MGUI::ClickEvent * e)
{
	Node * node = Editor::Instance()->GetSelectNode();
	if (!TYPE_OF(ParticleSystem, node))
		return ;
	//
	String filename = Util_GetSaveFile("", "Particle(*.particle)\0*.particle\0");
	if (filename != "")
	{
		String extName = FileHelper::GetExternName(filename);
		extName.ToLower();

		if (extName != "particle")
		{
			filename += ".particle";
		}

		FILE * fp = fopen(filename.c_str(), "wb");
		if (fp)
		{
			OSerializerF OS(fp, true);
			((ParticleSystem *)node)->SerializeTempalte(OS);
		}
	}
}

void ParticleFXPanel::OnReplay(const MGUI::ClickEvent * e)
{
	if (mParticleSystem != NULL)
	{
		mParticleSystem->Replay();
	}
}

void ParticleFXPanel::OnSetSelChanged(const MGUI::Event * e, int index)
{
	ParticleFXPropertyPanel::Instance()->AttachSet(NULL);
	ParticleFXPropertyPanel::Instance()->AttachEmitter(NULL);
	ParticleFXPropertyPanel::Instance()->AttachModifier(NULL);

	mListBox_Emitter->Clear();
	mListBox_Modifier->Clear();

	PS_Set * pSet = _getSelectSet();
	if (pSet)
	{
		for (int i = 0; i < pSet->GetEmitterCount(); ++i)
		{
			PS_Emitter * pEmitter = pSet->GetEmitter(i);

			mListBox_Emitter->Append(FixedString32(pEmitter->GetRTTI()->Name()).c_wstr(), pEmitter);
		}

		for (int i = 0; i < pSet->GetModifierCount(); ++i)
		{
			PS_Modifier * pModifier = pSet->GetModifier(i);

			mListBox_Modifier->Append(FixedString32(pModifier->GetRTTI()->Name()).c_wstr(), pModifier);
		}

		ParticleFXPropertyPanel::Instance()->AttachSet(pSet);
	}
}

void ParticleFXPanel::OnEmitterSelChanged(const MGUI::Event * e, int index)
{
	ParticleFXPropertyPanel::Instance()->AttachEmitter(NULL);

	if (index >= 0)
	{
		PS_Emitter * pEmitter = (PS_Emitter *)mListBox_Emitter->GetUserData(index);

		ParticleFXPropertyPanel::Instance()->AttachEmitter(pEmitter);
	}
}

void ParticleFXPanel::OnModifierSelChanged(const MGUI::Event * e, int index)
{
	ParticleFXPropertyPanel::Instance()->AttachModifier(NULL);

	if (index >= 0)
	{
		PS_Modifier * pModifier = (PS_Modifier *)mListBox_Modifier->GetUserData(index);

		ParticleFXPropertyPanel::Instance()->AttachModifier(pModifier);
	}
}

void ParticleFXPanel::OnAddSet(const MGUI::ClickEvent * e)
{
	mParticleNewSetDialog->E_EndDialog += new cListener1<ParticleFXPanel, bool>(this, &ParticleFXPanel::OnAddSet_End);
	mParticleNewSetDialog->DoModal();
}

void ParticleFXPanel::OnAddSet_End(bool _ok)
{
	if (!_ok || mParticleSystem == NULL)
		return ;

	String typeName = mParticleNewSetDialog->GetTypeName();

	if (typeName == "")
		return ;

	PS_Set * pSet = mParticleSystem->NewSet(typeName.c_str());
	if (pSet != NULL)
	{
		mListBox_Set->Append(typeName.c_wstr(), pSet);
	}
}

void ParticleFXPanel::OnDelSet(const MGUI::ClickEvent * e)
{
	int index = mListBox_Set->GetSelectIndex();
	if (index < 0 || mParticleSystem == NULL)
		return ;

	PS_Set * pSet = (PS_Set *)mListBox_Set->GetUserData(index);

	ParticleFXPropertyPanel::Instance()->AttachEmitter(NULL);
	ParticleFXPropertyPanel::Instance()->AttachModifier(NULL);

	mListBox_Set->Remove(index);

	pSet->GetParent()->DeleteSet(pSet);
}

void ParticleFXPanel::OnCopySet(const MGUI::ClickEvent * e)
{
	int index = mListBox_Set->GetSelectIndex();
	if (index < 0 || mParticleSystem == NULL)
		return ;

	PS_Set * pSet = (PS_Set *)mListBox_Set->GetUserData(index);

	ParticleFXPropertyPanel::Instance()->AttachEmitter(NULL);
	ParticleFXPropertyPanel::Instance()->AttachModifier(NULL);

	pSet = pSet->GetParent()->CloneSet(pSet);
	if (pSet != NULL)
	{
		String typeName = pSet->GetRTTI()->Name();

		mListBox_Set->Append(typeName.c_wstr(), pSet);
	}
}

void ParticleFXPanel::OnAddEmitter(const MGUI::ClickEvent * e)
{
	if (_getSelectSet() == NULL || !Editor::Instance()->IsLoaded() || mParticleSystem == NULL)
		return ;

	mParticleNewEmitterDialog->E_EndDialog += new cListener1<ParticleFXPanel, bool>(this, &ParticleFXPanel::OnAddEmitter_End);
	mParticleNewEmitterDialog->DoModal();
}

void ParticleFXPanel::OnAddEmitter_End(bool _ok)
{
	if (!_ok || mParticleSystem == NULL)
		return ;

	String typeName = mParticleNewEmitterDialog->GetTypeName();
	if (typeName == "")
		return ;

	PS_Set * pSet = _getSelectSet();
	if (pSet)
	{
		PS_Emitter * pEmitter = pSet->NewEmitter(typeName.c_str());
		if (pEmitter != NULL)
		{
			mListBox_Emitter->Append(FixedString32(pEmitter->GetRTTI()->Name()).c_wstr(), pEmitter);
		}
	}
}

void ParticleFXPanel::OnDelEmitter(const MGUI::ClickEvent * e)
{
	int index = mListBox_Emitter->GetSelectIndex();
	if (index < 0 || mParticleSystem == NULL)
		return ;

	PS_Set * pSet = _getSelectSet();
	if (pSet)
	{
		ParticleFXPropertyPanel::Instance()->AttachEmitter(NULL);

		pSet->DeleteEmitter(index);
		mListBox_Emitter->Remove(index);
	}
}

void ParticleFXPanel::OnAddModifier(const MGUI::ClickEvent * e)
{
	if (_getSelectSet() == NULL || !Editor::Instance()->IsLoaded() || mParticleSystem == NULL)
		return ;

	mParticleNewModifierDialog->E_EndDialog += new cListener1<ParticleFXPanel, bool>(this, &ParticleFXPanel::OnAddModifier_End);
	mParticleNewModifierDialog->DoModal();
}

void ParticleFXPanel::OnAddModifier_End(bool _ok)
{
	if (!_ok)
		return ;

	String typeName = mParticleNewModifierDialog->GetTypeName();
	if (typeName == "")
		return ;

	PS_Set * pSet = _getSelectSet();
	if (pSet)
	{
		PS_Modifier * pModifier = pSet->NewModifier(typeName.c_str());
		if (pModifier)
		{
			mListBox_Modifier->Append(FixedString32(pModifier->GetRTTI()->Name()).c_wstr(), pModifier);
		}
	}
}

void ParticleFXPanel::OnDelModifier(const MGUI::ClickEvent * e)
{
	int index = mListBox_Modifier->GetSelectIndex();
	if (index < 0 || mParticleSystem == NULL)
		return ;

	PS_Set * pSet = _getSelectSet();
	if (pSet)
	{
		ParticleFXPropertyPanel::Instance()->AttachModifier(NULL);

		pSet->DeleteModifier(index);
		mListBox_Modifier->Remove(index);
	}
}

PS_Set * ParticleFXPanel:: _getSelectSet()
{
	int index = mListBox_Set->GetSelectIndex();
	if (index < 0 || mParticleSystem == NULL)
		return NULL;

	PS_Set * pSet = (PS_Set *)mListBox_Set->GetUserData(index);

	return pSet;
}
