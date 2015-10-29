/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "EditorModule.h"
#include "MaterialPanel.h"

//
class MaterialModule : public EditorModule, public Singleton<MaterialModule>
{
public:
	MaterialModule ();
	virtual ~MaterialModule ();

	virtual void 
		Layout();
	virtual void 
		Show();
	virtual void 
		Hide();

	void
		Clear();

public:
	void
		OnNodeChanged();
	void 
		OnNodeDestroy(Node * node);
	void 
		OnSceneUnLoad();

protected:

	MaterialPanel * mPanel;
};

