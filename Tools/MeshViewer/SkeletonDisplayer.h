/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class SkeletonDisplayer
{
public:
	SkeletonDisplayer();
	~SkeletonDisplayer();

	void 
		Attach(Mesh * mesh);
	
	Skeleton *
		GetSkeleton() { return mSkeleton; }

protected:
	void 
		OnRender();

protected:
	Mesh * mMesh;
	Skeleton * mSkeleton;

	MGUI::Layout * mLayout;
	MGUI::TextBox * mTextBox;
};