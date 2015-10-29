/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "EditorModule.h"
#include "ComponentPanel.h"
#include "ComponentPropertyPanel.h"
#include "ComponentUserPanel.h"

class ComponentModule : public EditorModule
{
public:
	ComponentModule();
	virtual ~ComponentModule();

	virtual void Layout();
	virtual void Show();
	virtual void Hide();

protected:
	ComponentPanel * mPanel;
	ComponentPropertyPanel * mPropertyPanel;
	ComponentUserPanel * mUserPanel;
};
