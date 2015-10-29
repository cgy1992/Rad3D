/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MInclude.h"

class TrackerControl : public MGUI::Widget
{
public:
	TrackerControl(int steps, float minLength, float maxLength, MGUI::Widget * _parent);
	virtual ~TrackerControl();

	virtual void UpdateRenderItem(MGUI::Layout * _layout);

	float MapUnit(float abs_x);

protected:
	int mSteps;
	float mMinLength;
	float mMaxLength;
};

class TrackerPointer : public MGUI::CheckBox
{
public:
	TrackerPointer(MGUI::Widget * _parent);
	virtual ~TrackerPointer();

	virtual void UpdateRenderItem(MGUI::Layout * _layout);

	void SetStateColor(int index, const Float4 & color) { STATE_COLOR[index] = color; }

protected:
	Float4 STATE_COLOR[MGUI::eWidgetState::MAX];
};