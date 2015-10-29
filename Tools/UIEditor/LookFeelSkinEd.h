/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "RectEd.h"

class LookFeelSkinEd
{
public:
	LookFeelSkinEd();
	~LookFeelSkinEd();

	void Layout();
	MGUI::Layout * GetLayout() { return mLayout; }

protected:
	void OnEditSkinChanged_(Texture * _skin);
	void OnEditRectChanged_(MGUI::Rect _rect);
	void OnEditClientChanged_(MGUI::ClientRect _rect);
	void OnRectEdChanged_(MGUI::Rect _rect);

	void OnSkinMouseDown_(const MGUI::MouseEvent * e);
	void OnSkinMouseUp_(const MGUI::MouseEvent * e);
	void OnSkinMouseMove_(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;
	MGUI::ImageBox * mImageBox_Back;
	MGUI::ImageBox * mImageBox_Skin;

	RectEd * mRectEd;

	bool mEnableDrag;
	MGUI::Rect mTmpRect;
	float mTmpX, mTmpY;
};

