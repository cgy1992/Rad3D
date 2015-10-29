/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"
#include "OpenInFloderDlg.h"

class RenderViewer : public Singleton<RenderViewer>
{
	typedef FixedArray<String, 32> ItemGroup;

public:
	RenderViewer(int x, int y, int resolution);
	~RenderViewer();

	void Layout();

	void SetFloader(const String & floder, const String & key);

	bool CanPrev();
	bool CanNext();
	void OnPrevGroup();
	void OnNextGroup();

	void OnSearch(const String & key);

protected:
	void OnDispaly();
	void OnUpdateCamera(Camera * cam, Node * node);

	void OnResourceDrag(const MGUI::MouseEvent * e);
	void OnResourceOpen(const MGUI::MouseEvent * e);

protected:
	MGUI::Layout * mLayout;
	MGUI::Layout * mCenterLayout;

	Array<MGUI::ImageBox *> mImageBox;
	Array<MGUI::TextBox *> mTextBox;
	Array<Camera *> mCamera;
	Array<RenderContext *> mRenderContext;

	Array<Node *> mNode;
	Array<Node *> mObject;

	Array<ItemGroup> mItemGroupArray;
	int mCurrentGroupIndex;

	String mCurrentFloder;

	OpenInFloderDlg * mOpenInFloderDlg;
};