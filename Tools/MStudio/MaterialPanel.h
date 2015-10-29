/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "PropertyGrid.h"

struct eMaterialBlendMode : public IEnum
{
	DECLARE_ENUM(eMaterialBlendMode)

	enum enum_t {
		OPACITY,
		ALPHA_TEST,
		ALPHA_BLEND
	};
};

struct MaterialProperty : public IObject
{
	DECLARE_PROPERTY(IObject);

	Mesh * mesh;
	MeshBuffer * meshBuffer;

	FixedString32 shaderName;

	bool doubleSide;
	int blendMode;

	Float3 emissive;
	Float3 ambient;
	Float3 diffuse;
	Float3 specular;
	float shininess;

	String maps[eMapType::MAX];

	void
		Attach(MeshBuffer * mb);
	void
		OnPropertyChanged(const Property * p);
};

//
class MaterialPanel : public Singleton<MaterialPanel>
{
public:
	MaterialPanel();
	~MaterialPanel();

	MGUI::Layout *
		GetLayout() { return mLayout; }

	void 
		Layout();
	void 
		Show();
	void 
		Hide();

	void
		Attach(Mesh * mesh);

	void
		AttachShader(MeshShader * shader);

protected:
	void
		OnSubMeshChanged(const MGUI::Event * e, int index);
	void
		OnSave(const MGUI::ClickEvent * e);
	void
		OnRender();

protected:
	MGUI::Layout * mLayout;
	MGUI::Button * mBnSave;
	MGUI::ComboBox * mSubMeshList;
	PropertyGrid * mPropertyGrid;
	PropertyGrid * mPropertyGridShader;

	MaterialProperty mProperty;
};