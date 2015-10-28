/*
*	Shape
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MRay.h"
#include "MObject.h"

namespace Rad {

	class Node;

	class M_ENTRY Shape
	{
		DECLARE_RTTI_ROOT(Shape);

	public:
		Shape() : mOwner(NULL) { }
		virtual ~Shape() {}

		Node *
			GetOwner() { return mOwner; }

		virtual void
			OnAttach(Node * owner);
		virtual void 
			OnLoaded() = 0;
		virtual void 
			OnUnload() = 0;
		virtual void 
			OnUpdate(float elapsedTime) = 0;
		virtual void
			OnChangeTm(int flags) = 0;

		virtual bool
			OnRayCheck(float & dist, const Ray & ray, float length) = 0;

	protected:
		Node * mOwner;
	};

}