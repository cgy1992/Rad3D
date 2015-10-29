/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"
#include "NavPathFinder.h"

class NavGenerator : public Singleton<NavGenerator>
{
public:
	NavGenerator();
	~NavGenerator();

	void AddPoint(const Float3 & point, bool no_edge);
	void Connect(int index);
	void DeleteCurrentPoint();
	int PickPoint(const Ray & ray);
	void SelectPoint(int index);
	void DeleteEdge(const Ray & ray);
	void LookAtError();

	void OnDebugRender();

	bool GetSelectPoint(Float3 & point);
	void SetSelectPoint(const Float3 & point);

	void SetEditable(int em);
	bool IsEditable(){ return mEditMode != 0; }

	void Build();

	void SetTestPoint(const Float3 & point);

protected:
	void OnSceneLoaded();
	void OnSceneUnload();

protected:
	NavData * mData;
	int mSelectIndex;
	int mEditMode;

	int mTestIndex;
	Float3 mTestPoint1;
	Float3 mTestPoint2;

	NavPathFinderPtr mPathFinder;
};

