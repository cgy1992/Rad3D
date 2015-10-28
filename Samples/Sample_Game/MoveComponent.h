#pragma once

#include "stdafx.h"

class Actor;

class MoveCompoent : public IComponent
{
public:
	MoveCompoent();
	virtual ~MoveCompoent();

	void 
		SetTargetPosition(const Float3 & pos);
	const Float3 & 
		GetTargetPosition();
	const Float3 & 
		GetCurrentPosition();

	virtual int
		Update(float time);

protected:
	Actor * 
		GetActor() { return (Actor *)mOwner; }

protected:
	Float3 mTargetPos;
};
