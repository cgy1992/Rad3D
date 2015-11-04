/*
*	PS_SliceMesh
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"

namespace Rad {

	class FX_ENTRY PS_ShaderMesh : public PS_Shader
	{
		DECLARE_RTTI();

	public:
		PS_ShaderMesh();
		virtual ~PS_ShaderMesh();

		virtual void
			Update(float frameTime) {}

		virtual void
			OnCallBack(RenderObject * obj);
	};

}