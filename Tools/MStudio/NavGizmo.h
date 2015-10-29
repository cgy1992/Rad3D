/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class NavGizmo
{
public:
	tEvent1<Float3> E_PositionChanged;

public:
	NavGizmo();
	~NavGizmo();

	void Render();
	void Update(float mouseX, float mouseY);

	bool IsPicked() { return mPicked; }

	void SetEnable(bool enable) { mEnable = enable; }

protected:
	void _initGeo_Move();
	void _initGeo_Move_Render();

	void _renderMove();

	void _update_Move();
	void _mouseMoved_Move();

	float _getObjSize(const Float3 & point);

protected:
	ShaderFX * mTech;

	RenderOp * mRender_Move;
	int mNumVertex_Move;
	int mNumIndex_Move;
	Float3 * mVertex_Move;
	short * mIndex_Move;

	int mPickedAxis;
	bool mPicked;

	Float3 mOldPosition;

	float mMouseX;
	float mMouseY;

	bool mEnable;
};

