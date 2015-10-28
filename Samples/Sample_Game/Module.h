#pragma once

#include "Actor.h"

class Module
{
public:
	Module();
	virtual ~Module();

	virtual void
		Update(float frameTime);

	void 
		AddActor(Actor * obj);
	void 
		RemoveActor(Actor * obj);

protected:
	Array<Actor *> mActors;
};