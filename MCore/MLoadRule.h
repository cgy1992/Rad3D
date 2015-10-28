/*
*	LoadRule
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"

namespace Rad {

	class M_ENTRY ILoadRule
	{
	public:
		ILoadRule() {}
		virtual ~ILoadRule() {}

		virtual float 
			GetLoadPriority(Node * node, const Float3 & vPos) = 0;
	};

	class M_ENTRY LoadRuleStandard : public ILoadRule
	{
	public:
		LoadRuleStandard(float param);
		LoadRuleStandard();
		virtual ~LoadRuleStandard();

		void 
			SetRuleParam(float param) { mRuleParam = param; }
		float
			GetRuleParam() { return mRuleParam; }

		virtual float 
			GetLoadPriority(Node * node, const Float3 & pos);

	protected:
		float mRuleParam;
	};

}