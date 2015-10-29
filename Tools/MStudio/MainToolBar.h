#pragma once

#include "MStudioEntry.h"

class EntityToolBar
{
public:
	EntityToolBar();
	~EntityToolBar();

	void Layout();
	void Show();
	void Hide();

protected:
	void OnPick(const MGUI_MouseEvent * e);
	void OnMove(const MGUI_MouseEvent * e);
	void OnRotate(const MGUI_MouseEvent * e);
	void OnScale(const MGUI_MouseEvent * e);

	void OnPositionChanged(const MGUI_FocusEvent * e);
	void OnRotationChanged(const MGUI_FocusEvent * e);
	void OnScaleChanged(const MGUI_FocusEvent * e);

	void OnShapeSelectedChanged();
	void OnShapePositionChanged();
	void OnShapeRotationChanged();
	void OnShapeScaleChanged();

protected:
	MGUI_Layout * mLayout;

	MGUI_Widget * mWidget_Pick;
	MGUI_Widget * mWidget_Move;
	MGUI_Widget * mWidget_Rotate;
	MGUI_Widget * mWidget_Scale;

	MGUI_Layout * mLayout_Position;
	MGUI_Layout * mLayout_Rotation;
	MGUI_Layout * mLayout_Scale;

	MGUI_EditBox * mEditBox_PositionX;
	MGUI_EditBox * mEditBox_PositionY;
	MGUI_EditBox * mEditBox_PositionZ;

	MGUI_EditBox * mEditBox_RotationX;
	MGUI_EditBox * mEditBox_RotationY;
	MGUI_EditBox * mEditBox_RotationZ;

	MGUI_EditBox * mEditBox_Scale;
};

