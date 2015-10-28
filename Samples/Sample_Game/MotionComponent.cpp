#include "MotionComponent.h"
#include "Actor.h"

MotionComponent::MotionComponent()
{
	mCurrentState = eMotionState::NONE;
	mNextState = eMotionState::NONE;
	mCurrentState = eMotionState::NONE;
}

MotionComponent::~MotionComponent()
{
}

int MotionComponent::Update(float frameTime)
{
	_performStateChanged();

	_states(mCurrentState, eMotionEvent::UPDATE);

	return 0;
}

void MotionComponent::ChangeState(int state)
{
	mNextState = state;
	mStateChanged = TRUE;
}

int MotionComponent::GetCurrentState()
{
	return mCurrentState;
}

void MotionComponent::_performStateChanged()
{
	if (mStateChanged && mNextState != mCurrentState)
	{
		if (mCurrentState != eMotionState::NONE)
		{
			_states(mCurrentState, eMotionEvent::EXIT);
		}

		mCurrentState = mNextState;

		if (mCurrentState != eMotionState::NONE)
		{
			_states(mCurrentState, eMotionEvent::ENTER);
		}
	}

	mNextState = eMotionState::NONE;
	mStateChanged = FALSE;
}

bool MotionComponent::_states(int _state, int _event)
{
	Actor * actor = GetActor();

	E_DeclareState(eMotionState::IDLE) {

		E_Enter {

		}

		E_Exit {

		}

		E_Update {

			actor->PlayAnimation("idle");

		}
	}

	E_DeclareState(eMotionState::RUN) {

		E_Enter {

		}

		E_Exit {

		}

		E_Update {

			actor->PlayAnimation("run");

			Float3 d = actor->GetTargetPosition() - actor->GetPosition();
			if (d.LengthSq() < 0.01f)
			{
				actor->Idle();
			}
		}
	}

	return true;
}



