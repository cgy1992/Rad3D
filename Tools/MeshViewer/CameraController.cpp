#include "CameraController.h"

CameraController::CameraController()
{
	mMinDist = 0 * METER_LEN;
	mMaxDist = 20 * METER_LEN;

	mMinPitchAngle = 0;
	mMaxPitchAngle = 60;

	mDist = 12 * METER_LEN;
	mYawDegree = 0;
	mPitchDegree = 45;

	mLookAt = Float3(0, 0, 0);
}

CameraController::~CameraController()
{
}

void CameraController::ForceUpdate()
{
	Float3 targetPos = mLookAt;
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

void CameraController::SetLookAt(const Float3 & pos)
{
	mLookAt = pos;
}

void CameraController::SetYaw(float degree)
{
	mYawDegree = degree;
	mYawDegree = Math::DegreeNormalize(mYawDegree);
}

void CameraController::SetPitch(float degree)
{
	mPitchDegree = degree;
	mPitchDegree = Math::DegreeNormalize(mPitchDegree);
}

void CameraController::SetScroll(float dist)
{
	mDist = dist;
}