/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "EditorFactory.h"

class EntityPanel : public Singleton<EntityPanel>
{
public:
	EntityPanel();
	~EntityPanel();

	void Layout();
	void Show();
	void Hide();

	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	void InitEntityFactory();

	void OnUpdate(const MGUI::Event * e);

	void OnObjectPressed(const MGUI::MouseEvent *);
	void OnObjectReleased(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;

	MGUI::Layout * mLayoutTips;
	MGUI::TextBox * mTextBoxTips;

	MGUI::GridBox * mGridBox;

	EntityFactory * mCurrentFactory;
};