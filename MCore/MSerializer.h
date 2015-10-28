/*
*	Serializer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MField.h"
#include "MMath2.h"
#include "MString.h"
#include "MFixedString.h"
#include "MDataStream.h"

namespace Rad {

	class M_ENTRY Serializer
	{
	public:
		Serializer(int isOut) : mMode(isOut) {}
		virtual	~Serializer() {}

		bool
			IsOut() const { return mMode == 1; }

	protected:
		int mMode;
	};

}

#include "MISerializer.h"
#include "MOSerializer.h"