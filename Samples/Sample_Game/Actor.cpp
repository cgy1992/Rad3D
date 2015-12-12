#include "stdafx.h"

#include "Actor.h"

Actor::Actor(const char * filename)
{
	mMesh = new Mesh;
	mMesh->SetFilename(filename);

	mMoveSpeed = 3.0f * METER_LEN;
	mRotateSpeed = 360.0f * 2.0f;

	mMotionComponent = new MotionComponent;
	mMoveComponent = new MoveCompoent;
	mRotateComponent = new RotateComponent;
	mShadowComponent = new ShadowComponent;

	AddComponent(mMotionComponent);
	AddComponent(mMoveComponent);
	AddComponent(mRotateComponent);
	AddComponent(mShadowComponent);

	Idle();
}

Actor::~Actor()
{
	delete mMesh;
}

void Actor::Idle()
{
	mMotionComponent->ChangeState(eMotionState::IDLE);
}

void Actor::Run()
{
	mMotionComponent->ChangeState(eMotionState::RUN);
}

void Actor::MoveTo(const Float3 & pos, bool immediate)
{
	if (immediate)
	{
		_getNode()->SetPosition(pos);
		mMoveComponent->SetTargetPosition(pos);
	}
	else
	{
		const Float3 & myPos = GetPosition();
		Float3 dir = pos - myPos;

		if (dir.Normalize() > 0.01f)
		{
			float dt = dir.Dot(Float3(0, 0, 1));
			float deg = Math::ACos(dt);

			if (pos.x < myPos.x)
				deg = -deg;

			deg = Math::RadianToDegree(deg);

			RotateTo(deg);

			Run();

			mMoveComponent->SetTargetPosition(pos);
		}
	}
}

void Actor::RotateTo(float dir, bool immediate)
{
	dir = Math::DegreeNormalize(dir);

	if (immediate)
	{
		mRotateComponent->SetCurrentDirection(dir);
		mRotateComponent->SetTargetDirection(dir);
	}
	else
	{
		mRotateComponent->SetTargetDirection(dir);
	}
}

void Actor::PlayAnimation(const char * anim)
{
	if (!mMesh->IsAnimationPlaying(anim))
	{
		mMesh->PlayAnimation(anim);
	}
}

void Actor::Update(float dtime)
{
	UpdateComponent(dtime);
}

float Actor::GetDirection()
{
	return mRotateComponent->GetCurrentDirection();
}

float Actor::GetTargetDirection()
{
	return mRotateComponent->GetTargetDirection();
}

Float3 Actor::GetPosition()
{
	return mMoveComponent->GetCurrentPosition();
}

Float3 Actor::GetTargetPosition()
{
	return mMoveComponent->GetTargetPosition();
}

void Actor::SetShadowEnable(bool enable)
{
	if (enable)
	{
		if (mShadowComponent != NULL)
			return ;

		mShadowComponent = new ShadowComponent;
		AddComponent(mShadowComponent);
	}
	else
	{
		if (mShadowComponent != NULL)
		{
			RemoveComponent(mShadowComponent);
			mShadowComponent = NULL;
		}
	}
}

bool Actor::IsShadowEnable()
{
	return mShadowComponent != NULL;
}