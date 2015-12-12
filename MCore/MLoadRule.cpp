#include "MLoadRule.h"

namespace Rad {

	LoadRuleStandard::LoadRuleStandard(float param)
	{
		mRuleParam = param;
	}

	LoadRuleStandard::LoadRuleStandard()
	{
		mRuleParam = 100 * METER_LEN;
	}

	LoadRuleStandard::~LoadRuleStandard()
	{
	}

	float LoadRuleStandard::GetLoadPriority(Node * node, const Float3 & pos)
	{
		float maxDistSq = mRuleParam * mRuleParam;

		float distSq = pos.DistanceSq(node->GetPosition());

		return maxDistSq - distSq;
	}
}