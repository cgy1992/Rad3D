/*
*	PS_SliceStandard
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ShaderStandard : public PS_Shader
	{
		DECLARE_RTTI();

	public:
		PS_ShaderStandard();
		virtual ~PS_ShaderStandard();

		virtual void
			Update(float frameTime) {}

		virtual void
			OnCallBack(RenderObject * obj) {}
	};

}