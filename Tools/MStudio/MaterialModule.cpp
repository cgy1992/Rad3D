#include "MaterialModule.h"
#include "Editor.h"

ImplementSingleton(MaterialModule);

MaterialModule::MaterialModule()
	: EditorModule("Material")
{
	mPanel = new MaterialPanel;

	Editor::Instance()->E_NodeSelect += new cListener0<MaterialModule>(this, &MaterialModule::OnNodeChanged);
	Editor::Instance()->E_NodeDestroy += new cListener1<MaterialModule, Node *>(this, &MaterialModule::OnNodeDestroy);
	Editor::Instance()->E_SceneUnload += new cListener0<MaterialModule>(this, &MaterialModule::OnSceneUnLoad);

	Hide();
}

MaterialModule::~MaterialModule()
{
	Clear();

	delete mPanel;
}

void MaterialModule::Layout()
{
	mPanel->Layout();
}

void MaterialModule::Show()
{
	mPanel->Show();

	Editor::Instance()->ClearFlags(FLAG_ALL);
	Editor::Instance()->SetFlags(FLAG_PICK_ABLE);
}

void MaterialModule::Hide()
{
	mPanel->Hide();
}

void MaterialModule::Clear()
{
	mPanel->Attach(NULL);
}

void MaterialModule::OnNodeChanged()
{
	Clear();

	if (mPanel->GetLayout()->IsVisible())
	{
		Mesh * mesh = DYNAMIC_CAST(Mesh, Editor::Instance()->GetSelectNode());
		if (mesh != NULL && mesh->GetSource() != NULL)
		{
			mPanel->Attach(mesh);
		}
	}
}

void MaterialModule::OnNodeDestroy(Node * node)
{
	Clear();
}

void MaterialModule::OnSceneUnLoad()
{
	Clear();
}
