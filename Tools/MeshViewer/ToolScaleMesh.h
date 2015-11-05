#pragma once

#include "stdafx.h"

class ToolScaleMesh
{
public:
	ToolScaleMesh();
	~ToolScaleMesh();

	void 
		Show(MeshSourcePtr mesh);

protected:
	void 
		OnOK(const MGUI::ClickEvent * e);
	void 
		OnCancel(const MGUI::ClickEvent * e);

protected:
	MGUI::Layout * mLayout;
	MGUI::EditBox * mScaleBox;

	MeshSourcePtr mMesh;
};