#include "MaterialPanel.h"
#include "MaterialModule.h"
#include "MainFrame.h"
#include "Editor.h"
#include "MMeshSerializer.h"
#include "MessageBoxDialog.h"

//
DF_ENUM_BEGIN(eMaterialBlendMode)
	DF_ENUM(OPACITY)
	DF_ENUM(ALPHA_TEST)
	DF_ENUM(ALPHA_BLEND)
DF_ENUM_END()

//
DF_PROPERTY_BEGIN(MaterialProperty)
	DF_PROPERTY_EX(MaterialProperty, shaderName, "Material", "Shader", "PT_Class MeshShader",PT_FixedString)
	DF_PROPERTY_ENUM(MaterialProperty, blendMode, "Material", "BlendMode", eMaterialBlendMode)
	DF_PROPERTY_EX(MaterialProperty, emissive, "Material", "Emissive", "PT_Color", PT_Float3)
	DF_PROPERTY_EX(MaterialProperty, ambient, "Material", "Ambient", "PT_Color", PT_Float3)
	DF_PROPERTY_EX(MaterialProperty, diffuse, "Material", "Diffuse", "PT_Color", PT_Float3)
	DF_PROPERTY_EX(MaterialProperty, specular, "Material", "Specular", "PT_Color", PT_Float3)
	DF_PROPERTY(MaterialProperty, shininess, "Material", "Shininess", PT_Float)

	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::EMISSIVE], "Map", "Emissive", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::DIFFUSE], "Map", "Diffuse", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::NORMAL], "Map", "Normal", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::SPECULAR], "Map", "Specular", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::EXTERN0], "Map", "Extern0", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::EXTERN1], "Map", "Extern1", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::EXTERN2], "Map", "Extern2", "PT_Filename", PT_String)
	DF_PROPERTY_EX(MaterialProperty, maps[eMapType::EXTERN3], "Map", "Extern3", "PT_Filename", PT_String)
DF_PROPERTY_END()

void MaterialProperty::Attach(MeshBuffer * mb)
{
	meshBuffer = mb;

	shaderName = mb->GetShader() != NULL ? mb->GetShader()->GetRTTI()->Name() : "";

	Material * mtl = mb->GetMaterial();

	doubleSide = false;
	if (mtl->cullMode == eCullMode::NONE)
		doubleSide = true;

	blendMode = eMaterialBlendMode::OPACITY;
	switch(mtl->blendMode)
	{
	case eBlendMode::ALPHA_TEST:
		blendMode = eMaterialBlendMode::ALPHA_TEST;
		break;
		
	case eBlendMode::ALPHA_BLEND:
		blendMode = eMaterialBlendMode::ALPHA_BLEND;
		break;
	}

	emissive = mtl->emissive;
	ambient = mtl->ambient;
	diffuse = mtl->diffuse;
	specular = mtl->specular;
	shininess = mtl->shininess;

	for (int i = 0; i < eMapType::MAX; ++i)
	{
		maps[i] = mtl->maps[i] != NULL ? mtl->maps[i]->GetName() : "";
	}
}

void MaterialProperty::OnPropertyChanged(const Property * p)
{
	if (meshBuffer == NULL)
		return ;

	if (p->name == "shaderName")
	{
		meshBuffer->SetShaderClass(shaderName.c_str());

		for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
		{
			if (mesh->GetSubMesh(i)->GetMeshBuffer() == meshBuffer)
			{
				mesh->GetSubMesh(i)->SetMeshShader(meshBuffer->GetShader());
				MaterialPanel::Instance()->AttachShader(meshBuffer->GetShader().c_ptr());
				break;
			}
		}
	}
	else
	{
		Material * mtl = meshBuffer->GetMaterial();

		if (doubleSide)
			mtl->cullMode = eCullMode::NONE;
		else
			mtl->cullMode = eCullMode::BACK;

		mtl->depthMode = eDepthMode::LESS_EQUAL;

		switch (blendMode)
		{
		case eMaterialBlendMode::OPACITY:
			mtl->blendMode = eBlendMode::OPACITY;
			break;

		case eMaterialBlendMode::ALPHA_TEST:
			mtl->blendMode = eBlendMode::ALPHA_TEST;
			break;

		case eMaterialBlendMode::ALPHA_BLEND:
			mtl->blendMode = eBlendMode::ALPHA_BLEND;
			mtl->depthMode = eDepthMode::N_LESS_EQUAL;
			break;
		}

		mtl->ambient = ambient;
		mtl->diffuse = diffuse;
		mtl->specular = specular;
		mtl->shininess = shininess;

		for (int i = 0; i < eMapType::MAX; ++i)
		{
			if (maps[i] != "")
			{
				mtl->maps[i] = HWBufferManager::Instance()->LoadTexture(maps[i], -1);
			}
			else
			{
				mtl->maps[i] = NULL;
			}
		}

		for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
		{
			if (mesh->GetSubMesh(i)->GetMeshBuffer() == meshBuffer)
			{
				TexturePtr lightingmap = mesh->GetSubMesh(i)->GetMaterial()->maps[eMapType::LIGHTING_MAP];
				*(mesh->GetSubMesh(i)->GetMaterial()) = *(meshBuffer->GetMaterial());
				mesh->GetSubMesh(i)->GetMaterial()->maps[eMapType::LIGHTING_MAP] = lightingmap;
				break;
			}
		}
	}
}

//
ImplementSingleton(MaterialPanel);

MaterialPanel::MaterialPanel()
{
	mProperty.mesh = NULL;
	mProperty.meshBuffer = NULL;

	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel(), NULL);
	mLayout->SetRect(0, 0, 256 + 128, 256);
	d_assert(mLayout != NULL);

	mSubMeshList = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mSubMeshList->SetRect(0, 0, 256, 27);
	mSubMeshList->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mSubMeshList->SetDockable(true);
	mSubMeshList->E_SelectChanged += new cListener2<MaterialPanel, const MGUI::Event *, int>(this, &MaterialPanel::OnSubMeshChanged);

	mPropertyGrid = new PropertyGrid(mLayout);
	mPropertyGrid->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGrid->SetDockable(true);

	mPropertyGridShader = new PropertyGrid(mLayout);
	mPropertyGridShader->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mPropertyGridShader->SetDockable(true);

	mBnSave = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	mBnSave->SetRect(0, 0, 256, 27);
	mBnSave->SetAlign(MGUI::eAlign::TOP | MGUI::eAlign::H_STRETCH);
	mBnSave->SetCaption(WS("Save"));
	mBnSave->SetDockable(true);
	mBnSave->E_Click += new cListener1<MaterialPanel, const MGUI::ClickEvent *>(this, &MaterialPanel::OnSave);

	World::Instance()->E_RenderSolid += new cListener0<MaterialPanel>(this, &MaterialPanel::OnRender);
}

MaterialPanel::~MaterialPanel()
{
	delete mPropertyGridShader;
	delete mPropertyGrid;
	delete mLayout;
}

void MaterialPanel::Layout()
{
	MGUI::Rect rect = MGUI::Engine::Instance()->GetRect();

	float w = mLayout->GetRect().w;
	mLayout->SetRect(0, D_MAINMENU_H, w, rect.h - D_MAINMENU_H - D_MAINSTATUSBAR_H);
}

void MaterialPanel::Show()
{
	mLayout->SetVisible(true);

	Mesh * mesh = DYNAMIC_CAST(Mesh, Editor::Instance()->GetSelectNode());
	if (mesh != NULL && mesh->GetSource() != NULL)
	{
		if (mProperty.mesh != mesh)
		{
			Attach(mesh);
		}
	}
	else
	{
		Attach(NULL);
	}
}

void MaterialPanel::Hide()
{
	mLayout->SetVisible(false);
}

void MaterialPanel::Attach(Mesh * mesh)
{
	mSubMeshList->Clear();
	mPropertyGrid->Clear();
	mPropertyGridShader->Clear();

	mProperty.mesh = mesh;
	mProperty.meshBuffer = NULL;

	if (mesh)
	{
		for (int i = 0; i < mesh->GetSubMeshCount(); ++i)
		{
			SubMesh * sm = mesh->GetSubMesh(i);

			if (sm->GetMeshBuffer() != NULL)
			{
				FixedString32 name = sm->GetMeshBuffer()->GetName();
				name.Trim();

				if (name == "")
				{
					name.Format("SubMesh%d", i);
				}

				mSubMeshList->Append(name.c_wstr(), sm);
			}
		}
	}
}

void MaterialPanel::AttachShader(MeshShader * shader)
{
	mPropertyGridShader->Attach(shader);
}

void MaterialPanel::OnSubMeshChanged(const MGUI::Event * e, int index)
{
	if (index != -1)
	{
		SubMesh * sm = (SubMesh *)mSubMeshList->GetUserData(index);
		mProperty.Attach(sm->GetMeshBuffer());
		mPropertyGrid->Attach(&mProperty);

		AttachShader(sm->GetMeshShader().c_ptr());
	}
}

void MaterialPanel::OnSave(const MGUI::ClickEvent * e)
{
	if (mProperty.mesh != NULL)
	{
		MeshSourcePtr source = mProperty.mesh->GetSource();

		String filename = ResourceManager::Instance()->GetAbsoluteSourceName(source->GetSourceName());

		if (MeshSerializer::Save(source.c_ptr(), filename))
		{
			DoMessageBox("Save Mesh OK.", MessageBoxType::Info);
		}
		else
		{
			DoMessageBox("Save Mesh Failed.", MessageBoxType::Error);
		}
	}
}

void MaterialPanel::OnRender()
{
	if (mLayout->IsVisible() && mProperty.mesh != NULL && mProperty.meshBuffer != NULL)
	{
		for (int i = 0; i < mProperty.mesh->GetSubMeshCount(); ++i)
		{
			SubMesh * submesh = mProperty.mesh->GetSubMesh(i);
			if (submesh->GetMeshBuffer() == mProperty.meshBuffer)
			{
				Mat4 worldTm, scaleTm;

				worldTm = mProperty.mesh->GetWorldTM();
				scaleTm.MakeScale(1.001f, 1.001f, 1.001f);

				RenderSystem::Instance()->SetRenderState(eFillMode::FRAME, eCullMode::NONE);

				RenderHelper::Instance()->DebugDraw(submesh->GetRenderOp(), Float4(0, 1, 0), scaleTm * worldTm);

				break;
			}
		}
	}
}
