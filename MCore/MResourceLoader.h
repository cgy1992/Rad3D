/*
*	ResourceLoader
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MResource.h"

namespace Rad {

	class M_ENTRY ResourceLoader
	{
		DECLARE_ALLOC();

	public:
		ResourceLoader();
		virtual ~ResourceLoader();

		virtual void 
			Load(ResourcePtr pResource);
		virtual void 
			ForceLoad(ResourcePtr pResource);

		virtual bool
			IsMultiThread() { return false; }
	};

}