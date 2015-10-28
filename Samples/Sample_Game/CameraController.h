#pragma once

#include "stdafx.h"

class Actor;

class CameraController : public IComponent
{
public:
	CameraController();
	virtual ~CameraController();

	virtual int
		Update(float frameTime);

	void 
		ForceUpdate();

	void 
		Yaw(float degree);
	void 
		Pitch(float degree);
	void 
		Scroll(float dist);

protected:
	Actor * 
		GetActor() { return (Actor *)mOwner; }

protected:
	float mMinDist;
	float mMaxDist;

	float mMinPitchAngle;
	float mMaxPitchAngle;

	float mHeadHeight;

	float mDist;
	float mYawDegree;
	float mPitchDegree;
};


