#include "ParticleFXPropertyPanel.h"
#include "MainFrame.h"
#include "Editor.h"
#include "ParticleFXPanel.h"
#include "PropertyField_KeyFrame.h"
#include "ParticleFXUserPanel.h"

class ParticleFX_PropertyFieldCreator : public IPropertyGrid_UserCreator
{
public:
	ParticleFX_PropertyFieldCreator() {}
	virtual ~ParticleFX_PropertyFieldCreator() {}

	PropertyField * CreatePropertyField(PropertyGroup * group, IObject * obj, const Property * prop)
	{
		if (strcmp(prop->editorName, "PT_KeyController") == 0)
		{
			return new PropertyFiled_KeyFrame(group, obj, prop, ParticleFXUserPanel::Instance()->GetLayout());
		}

		return NULL;
	}
};

ParticleFX_PropertyFieldCreator gParticleFXPropertyFieldCreator;

ImplementSingleton(ParticleFXPropertyPanel);

ParticleFXPropertyPanel::ParticleFXPropertyPanel()
{
	mLayout = MGUI::Layout::Load("ParticleFXPropertyPanel.layout", NULL);
	d_assert(mLayout != NULL);

	mLayout->SetVisible(false);

	mTopWidget = mLayout->GetChild("Mode");

	MGUI::ComboBox * pComboBox_Mode = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	pComboBox_Mode->SetRect(mTopWidget->GetRect());
	pComboBox_Mode->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	pComboBox_Mode->SetDockable(true);
	pComboBox_Mode->Append(WSTR_("Set"));
	pComboBox_Mode->Append(WSTR_("Emitter"));
	pComboBox_Mode->Append(WSTR_("Modifier"));
	pComboBox_Mode->SetSelectIndex(0);
	pComboBox_Mode->E_SelectChanged += new cListener2<ParticleFXPropertyPanel, const MGUI::Event *, int>(this, &ParticleFXPropertyPanel::OnModeChanged);

	mPropertyGrid_Set = new PropertyGrid(mLayout);
	mPropertyGrid_Set->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid_Set->SetDockable(true);
	mPropertyGrid_Set->SetVisible(true);

	mPropertyGrid_Shader = new PropertyGrid(mLayout);
	mPropertyGrid_Shader->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid_Shader->SetDockable(true);
	mPropertyGrid_Shader->SetVisible(true);

	mPropertyGrid_Emitter = new PropertyGrid(mLayout);
	mPropertyGrid_Emitter->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid_Emitter->SetDockable(true);
	mPropertyGrid_Emitter->SetVisible(false);

	mPropertyGrid_Modifier = new PropertyGrid(mLayout);
	mPropertyGrid_Modifier->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid_Modifier->SetDockable(true);
	mPropertyGrid_Modifier->SetVisible(false);

	Editor::Instance()->E_Update += new cListener0<ParticleFXPropertyPanel>(this, &ParticleFXPropertyPanel::OnUpdate);
}

ParticleFXPropertyPanel::~ParticleFXPropertyPanel()
{
	delete mPropertyGrid_Set;
	delete mPropertyGrid_Emitter;
	delete mPropertyGrid_Modifier;
	delete mPropertyGrid_Shader;
	safe_delete (mLayout);
}

void ParticleFXPropertyPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;
	mLayout->SetRect(ParticleFXPanel::Instance()->GetLayout()->GetRect().w, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void ParticleFXPropertyPanel::Show()
{
	mLayout->SetVisible(true);
}

void ParticleFXPropertyPanel::Hide()
{
	mLayout->SetVisible(false);
}

void ParticleFXPropertyPanel::AttachSet(PS_Set * pSet)
{
	mPropertyGrid_Set->Attach(pSet, &gParticleFXPropertyFieldCreator);
	mPropertyGrid_Shader->Attach(NULL, &gParticleFXPropertyFieldCreator);
}

void ParticleFXPropertyPanel::AttachEmitter(PS_Emitter * pEmitter)
{
	mPropertyGrid_Emitter->Attach(pEmitter, &gParticleFXPropertyFieldCreator);
}

void ParticleFXPropertyPanel::AttachModifier(PS_Modifier * pModifier)
{
	mPropertyGrid_Modifier->Attach(pModifier, &gParticleFXPropertyFieldCreator);
}

void ParticleFXPropertyPanel::OnModeChanged(const MGUI::Event * e, int index)
{
	mPropertyGrid_Set->SetVisible(false);
	mPropertyGrid_Emitter->SetVisible(false);
	mPropertyGrid_Modifier->SetVisible(false);
	mPropertyGrid_Shader->SetVisible(false);

	switch (index)
	{
	case 0:
		mPropertyGrid_Set->SetVisible(true);
		mPropertyGrid_Shader->SetVisible(true);
		break;

	case 1:
		mPropertyGrid_Emitter->SetVisible(true);
		break;

	case 2:
		mPropertyGrid_Modifier->SetVisible(true);
		break;
	}
}

void ParticleFXPropertyPanel::OnUpdate()
{
	if (!mLayout->IsVisible())
		return ;

	PS_Set * render = (PS_Set *)mPropertyGrid_Set->GetAttachObj();
	if (render != NULL && render->GetShader().c_ptr() != mPropertyGrid_Shader->GetAttachObj())
	{
		mPropertyGrid_Shader->Attach(render->GetShader().c_ptr());
	}
}