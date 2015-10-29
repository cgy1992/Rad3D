/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class Gizmo : public Singleton<Gizmo>
{
public:
	enum eOperator {
		OP_UNKONWN,
		OP_MOVE,
		OP_ROTATE,
		OP_SCALE
	};

public:
	Gizmo();
	~Gizmo();

	void 
		Update(float mouseX, float mouseY);

	void
		SetOperator(int op) { mOperator = op; }
	int
		GetOperator() { return mOperator; }

protected:
	void 
		OnRender();

	bool 
		IsPicked() { return mPicked; }

protected:
	void _initGeo_Move();
	void _initGeo_Move_Render();
	void _initGeo_Rotate();
	void _initGeo_Rotate_Render();
	void _initGeo_Scale();
	void _initGeo_Scale_Render();

	void _renderMove();
	void _renderRotate();
	void _renderScale();

	void _update_Move();
	void _mouseMoved_Move();

	void _update_Rotate();
	void _mouseMoved_Rotate();

	void _update_Scale();
	void _mouseMoved_Scale();

	float _getObjSize(Node * obj);

protected:
	ShaderFX * mTech;
	int mOperator;

	RenderOp * mRender_Move;
	int mNumVertex_Move;
	int mNumIndex_Move;
	Float3 * mVertex_Move;
	short * mIndex_Move;

	RenderOp * mRender_Rotate;
	int mNumVertex_Rotate;
	int mNumIndex_Rotate;
	Float3 * mVertex_Rotate;
	short * mIndex_Rotate;

	RenderOp * mRender_Scale;
	int mNumVertex_Scale;
	int mNumIndex_Scale;
	Float3 * mVertex_Scale;
	short * mIndex_Scale;

	int mPickedAxis;
	bool mPicked;

	Float3 mOldPosition;
	Quat mOldRotation;
	Float3 mOldScale;

	float mMouseX;
	float mMouseY;
};

