/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class GizmoBar
{
public:
	GizmoBar();
	~GizmoBar();

	void 
		Layout();

	void
		Show() { mLayout->SetVisible(true); }
	void
		Hide() { mLayout->SetVisible(false); }
	MGUI::Layout *
		GetLayout() { return mLayout; }

protected:
	void 
		OnMove(const MGUI::MouseEvent * e);
	void 
		OnRotate(const MGUI::MouseEvent * e);
	void 
		OnScale(const MGUI::MouseEvent * e);

	void 
		OnTextChanged(const MGUI::FocusEvent * e);

	void
		OnNodeSelect();
	void
		OnPositionChanged();
	void
		OnRotationChanged();
	void
		OnScaleChanged();

protected:
	MGUI::Layout * mLayout;

	MGUI::Widget * mWidget_Move;
	MGUI::Widget * mWidget_Rotate;
	MGUI::Widget * mWidget_Scale;

	MGUI::EditBox * mEditBox_X;
	MGUI::EditBox * mEditBox_Y;
	MGUI::EditBox * mEditBox_Z;
};