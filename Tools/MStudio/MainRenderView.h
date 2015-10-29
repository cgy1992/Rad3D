/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class MSTUDIO_ENTRY MainRenderView : public Singleton<MainRenderView>
{
public:
	MainRenderView();
	~MainRenderView();

	void 
		Layout();
	bool 
		IsMouseFoucsed();
	
	MGUI::Layout * 
		GetLayout() { return mLayout; }

protected:
	void
		OnUpdate();
	void
		OnDoubleClick(const MGUI::MouseEvent * e);
	void
		OnMouseDown(const MGUI::MouseEvent * e);
	void
		OnMouseUp(const MGUI::MouseEvent * e);
	void
		OnMouseDrag(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;
	
	bool mRangePick;
	Float2 mMousePos1;
	Float2 mMousePos2;

	MGUI::Widget * mRectWidget;
	MGUI::Widget * mLeft;
	MGUI::Widget * mRight;
	MGUI::Widget * mTop;
	MGUI::Widget * mBottom;
};

