#include "CameraController.h"
#include "Actor.h"

CameraController::CameraController()
{
	mMinDist = 5 * METER_LEN;
	mMaxDist = 20 * METER_LEN;

	mMinPitchAngle = 0;
	mMaxPitchAngle = 60;

	mHeadHeight = 0;

	mDist = 10 * METER_LEN;
	mYawDegree = 0;
	mPitchDegree = 45;
}

CameraController::~CameraController()
{
}

int CameraController::Update(float frameTime)
{
	Actor * pActor = GetActor();

	Float3 targetPos = pActor->_getNode()->GetPosition();
	Quat targetOrt = Quat::Identity;

	float dist = mDist;
	float pitch = Math::DegreeToRadian(mPitchDegree);
	float yaw = Math::DegreeToRadian(mYawDegree);

	Quat q0, q1;

	q0.FromAxis(Float3(1, 0, 0), pitch);
	q1.FromAxis(Float3(0, 1, 0), yaw);

	targetOrt = q1 * (q0 * targetOrt);
	targetPos = targetPos - mDist * targetOrt.GetDirVector();

	Float3 origPos = World::Instance()->MainCamera()->GetPosition();
	Float3 dir = targetPos - origPos;
	float k = Math::Clamp(frameTime * 15.0f, 0.0f, 1.0f);
	targetPos = origPos + dir * k;

	World::Instance()->MainCamera()->SetRotation(targetOrt);
	World::Instance()->MainCamera()->SetPosition(targetPos);

	return 0;
}

void CameraController::ForceUpdate()
{
	Actor * pActor = GetActor();

	Float3 targetPos = pActor->_getNode()->GetPosition();
	Quat targetOrt = Quat::Identity;

	float dist = mDist;
	float pitch = Math::DegreeToRadian(mPitchDegree);
	float yaw = Math::DegreeToRadian(mYawDegree);

	Quat q0, q1;

	q0.FromAxis(Float3(1, 0, 0), pitch);
	q1.FromAxis(Float3(0, 1, 0), yaw);

	targetOrt = q1 * (q0 * targetOrt);
	targetPos = targetPos - mDist * targetOrt.GetDirVector();

	World::Instance()->MainCamera()->SetRotation(targetOrt);
	World::Instance()->MainCamera()->SetPosition(targetPos);
}

void CameraController::Yaw(float rads)
{
	mYawDegree += rads;
	mYawDegree = Math::DegreeNormalize(mYawDegree);
}

void CameraController::Pitch(float rads)
{
	mPitchDegree += rads;
	mPitchDegree = Math::DegreeNormalize(mPitchDegree);
}

void CameraController::Scroll(float dist)
{
	mDist = Math::Clamp(mDist + dist, mMinDist, mMaxDist);
}
