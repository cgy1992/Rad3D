/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "RectEd.h"

class LayoutViewEd
{
public:
	LayoutViewEd();
	~LayoutViewEd();

	void Layout();
	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	MGUI::Layout * mLayout;
	MGUI::ImageBox * mImageBox_Back;
	RectEd * mRectEd;

protected:
	void OnRectEdChanged_(MGUI::Rect rect);
	void OnEditRectChanged_(MGUI::Rect rect);
};
