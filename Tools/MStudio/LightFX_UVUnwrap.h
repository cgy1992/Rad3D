/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"
#include "LightFX_Mesh.h"

namespace Rad {

	void FX_Unwarp_Init();
	void FX_Unwarp_Shutdown();
	bool FX_Unwarp(FX_Mesh * fxmesh);

	class FX_UVUnwrapThread : public Thread
	{
	public:
		tEvent1<int> E_Progress;

	public:
		FX_UVUnwrapThread(const Array<FX_Mesh *> & arr);
		virtual ~FX_UVUnwrapThread();

		virtual void 
			Run();

	protected:
		Array<FX_Mesh *> mMeshes;
	};
}