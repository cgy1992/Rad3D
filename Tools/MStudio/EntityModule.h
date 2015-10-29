/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "EditorModule.h"
#include "EntityPanel.h"
#include "EntityPropertyPanel.h"
#include "EntityBound.h"
#include "GizmoBar.h"

class EntityModule : public EditorModule, public Singleton<EntityModule>
{
public:
	EntityModule();
	virtual ~EntityModule();

	virtual void 
		Layout();
	virtual void 
		Show();
	virtual void 
		Hide();

	void 
		AddCreator(EntityFactory * creator);

protected:
	EntityPanel * mEntityPanel;
	EntityPropertyPanel * mEntityPropertyPanel;

	EntityBound * mEntityBound;

	GizmoBar * mGizmoBar;
};