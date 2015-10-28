#pragma once

#include "stdafx.h"

class Actor;

class RotateComponent : public IComponent
{
public:
	RotateComponent();
	virtual ~RotateComponent();

	void 
		SetTargetDirection(float dir);
	float 
		GetTargetDirection();
	
	void 
		SetCurrentDirection(float dir);
	float 
		GetCurrentDirection();

	virtual int
		Update(float time);

protected:
	Actor * GetActor() { return (Actor *)mOwner; }

protected:
	float mDirection;
	float mTargetDir;
};