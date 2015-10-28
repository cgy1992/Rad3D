/*
*	VisibleCuller
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MLight.h"
#include "MCamera.h"

namespace Rad {

	class M_ENTRY VisibleCuller : public IObject
	{
		DECLARE_REF();
		DECLARE_RTTI();

	public:
		VisibleCuller();
		virtual ~VisibleCuller();

		void
			Release() { delete this; }

		virtual void 
			DoCull(int frameId, Camera * camera) = 0;

		Array<Node *> & 
			GetNodeArray() { return mNodeArray; }
		Array<Light *> & 
			GetLightArray() { return mLightArray; }

	protected:
		int mFrameId;
		Array<Node *> mNodeArray;
		Array<Light *> mLightArray;
	};

	typedef SmartPtr<VisibleCuller> VisibleCullerPtr;

	//
	class M_ENTRY VisibleCullerStandard : public VisibleCuller
	{
		DECLARE_RTTI();

	public:
		VisibleCullerStandard();
		virtual ~VisibleCullerStandard();

		virtual void 
			DoCull(int frameId, Camera * camera);
	};

}