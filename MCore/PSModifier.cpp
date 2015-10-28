#include "PSModifier.h"

namespace Rad {

	ImplementRTTI(PS_Modifier, IObject);

	DF_PROPERTY_BEGIN(PS_Modifier)
		DF_PROPERTY(PS_Modifier, mEnable, "", "Enable", PT_Bool)
	DF_PROPERTY_END()

	PS_Modifier::PS_Modifier()
		: mParent(NULL)
		, mEnable(true)
	{
	}

	PS_Modifier::~PS_Modifier()
	{
	}

	void PS_Modifier::Init(PS_Set * parent)
	{
		mParent = parent;
	}

	void PS_Modifier::SetEnable(bool enable)
	{
		mEnable = enable;
	}

	bool PS_Modifier::IsEnable()
	{
		return mEnable;
	}

}