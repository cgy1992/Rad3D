#pragma once

#include "MotionComponent.h"
#include "MoveComponent.h"
#include "RotateComponent.h"
#include "ShadowComponent.h"

class Actor : public ComponentOwner
{
public:
	Actor(const char * filename);
	virtual ~Actor();

	virtual void 
		Update(float frameTime);

	virtual void 
		Idle();
	virtual void 
		Run();

	virtual void 
		MoveTo(const Float3 & pos, bool immediate = false);
	virtual void 
		RotateTo(float dir, bool immediate = false);

	void
		PlayAnimation(const char * anim);

	//
	void 
		SetMoveSpeed(float speed) { mMoveSpeed = speed; }
	float 
		GetMoveSpeed() const { return mMoveSpeed; }

	void 
		SetRotateSpeed(float speed) { mRotateSpeed = speed; }
	float 
		GetRotateSpeed() const { return mRotateSpeed; }

	void 
		SetInitOrientation(const Quat & ort) { mInitOrt = ort; }
	const Quat & 
		GetInitOrientation() const { return mInitOrt; }

	float 
		GetDirection();
	float 
		GetTargetDirection();
	Float3 
		GetPosition();
	Float3 
		GetTargetPosition();
	
	Node * 
		_getNode() { return mMesh; }

	void 
		SetShadowEnable(bool enable);
	bool 
		IsShadowEnable();

protected:
	Mesh * mMesh;

	float mMoveSpeed;
	float mRotateSpeed;

	Quat mInitOrt;

	MotionComponent * mMotionComponent;
	MoveCompoent * mMoveComponent;
	RotateComponent * mRotateComponent;
	ShadowComponent * mShadowComponent;
};

	
