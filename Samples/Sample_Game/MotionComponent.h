#pragma once

#include "stdafx.h"

class Actor;

struct eMotionState {

	enum {
		NONE,
		IDLE,
		RUN,
	};
};

struct eMotionEvent {

	enum {
		NONE,
		ENTER,
		EXIT,
		UPDATE,
	};
};

#define E_DeclareState(state) \
		if (_state == state)

#define E_Enter \
		if (_event == eMotionEvent::ENTER)

#define E_Exit \
		if (_event == eMotionEvent::EXIT) 

#define E_Update \
		if (_event == eMotionEvent::UPDATE)


class MotionComponent : public IComponent
{
public:
	MotionComponent();
	virtual ~MotionComponent();

	virtual int 
		Update(float frameTime);

	void 
		ChangeState(int state);
	int 
		GetCurrentState();

protected:
	Actor * 
		GetActor() { return (Actor *) mOwner; }

	virtual void 
		_performStateChanged();
	virtual bool 
		_states(int _state, int _event);

protected:
	int mCurrentState;
	int mNextState;

	int mStateChanged;
};
