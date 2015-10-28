#include "RotateComponent.h"
#include "Actor.h"

RotateComponent::RotateComponent()
	: mDirection(0)
{
}

RotateComponent::~RotateComponent()
{
}

void RotateComponent::SetTargetDirection(float dir)
{
	mTargetDir = dir;
}

float RotateComponent::GetTargetDirection()
{
	return mTargetDir;
}

void RotateComponent::SetCurrentDirection(float dir)
{
	mDirection = dir;
}

float RotateComponent::GetCurrentDirection()
{
	return mDirection;
}

int RotateComponent::Update(float time)
{
	Actor * pActor = GetActor();

	float curDir = mDirection;
	float tarDir = mTargetDir;
	float speed = pActor->GetRotateSpeed() * time;
	float dir = curDir;

	if (tarDir > curDir + 180)
	{
		curDir += 360;
	}

	if (curDir > tarDir + 180)
	{
		tarDir += 360;
	}

	float dk = tarDir - curDir;

	if (Math::Abs(dk) < speed)
	{
		dir = mTargetDir;
	}
	else
	{
		if (dk > 0)
			dk = 1;
		else
			dk = -1;

		dir += dk * speed;
	}

	mDirection = Math::DegreeNormalize(dir);

	Quat quat;
	quat.FromAxis(Float3(0, 1, 0), Math::DegreeToRadian(mDirection));

	pActor->_getNode()->SetRotation(pActor->GetInitOrientation() * quat);

	return 0;
}