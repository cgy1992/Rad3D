#include "MoveComponent.h"
#include "Actor.h"
#include "Engine.h"

MoveCompoent::MoveCompoent()
{
}

MoveCompoent::~MoveCompoent()
{
}

void MoveCompoent::SetTargetPosition(const Float3 & pos)
{
	mTargetPos = pos;
}

const Float3 & MoveCompoent::GetTargetPosition()
{
	return mTargetPos;
}

const Float3 & MoveCompoent::GetCurrentPosition()
{
	return GetActor()->_getNode()->GetPosition();
}

int MoveCompoent::Update(float time)
{
	Actor * pActor = GetActor();

	Float3 curPos = pActor->_getNode()->GetPosition();
	Float3 tarPos = mTargetPos;
	float speed = pActor->GetMoveSpeed() * time;

	if (mTargetPos.Distance(curPos) <= speed)
	{
		pActor->_getNode()->SetPosition(mTargetPos);
	}
	else
	{
		Float3 dir = tarPos - curPos;
		dir.Normalize();

		curPos += dir * speed;
		pActor->_getNode()->SetPosition(curPos);

		GrassManager::Instance()->PushWave(curPos.x, curPos.z, 2.0f);
	}

	Float3 pos = pActor->_getNode()->GetPosition();
	pos.y = Engine::Instance()->GetHeightAt(pos);

	pActor->_getNode()->SetPosition(pos);

	return 0;
}


