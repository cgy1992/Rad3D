/*
*	Modifier Key Controller
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ModifierKeyController : public PS_Modifier
	{
		DECLARE_RTTI();

	public:
		PS_ModifierKeyController();
		virtual ~PS_ModifierKeyController();

		virtual void
			Serialize(Serializer & sl);

		virtual IKeyController * 
			GetKeyController()  = 0;
	};

}