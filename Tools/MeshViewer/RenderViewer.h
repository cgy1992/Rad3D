/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class RenderViewer
{
public:
	RenderViewer();
	~RenderViewer();

	void 
		Layout();
	MGUI::Layout *
		GetLayout() { return mLayout; }

	bool 
		IsMouseFocused();

protected:
	void
		OnUpdate();

protected:
	MGUI::Layout * mLayout;
	MGUI::TextBox * mTextBox;

	Mesh * mGridMesh;
};