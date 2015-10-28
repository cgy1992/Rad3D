/*
*	Modifier Deflector Plane
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "PSTypes.h"
#include "MPlane.h"

namespace Rad {

	class FX_ENTRY PS_ModifierDeflectorPlane : public PS_Modifier
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(PS_Modifier);

	public:
		PS_ModifierDeflectorPlane();
		virtual ~PS_ModifierDeflectorPlane();

		void 
			SetPlane(const Plane & plane);
		const Plane & 
			GetPlane() const { return mPlane; }

		void 
			SetBounce(float bounce);
		float 
			GetBounce() const { return mBounce; }

		virtual void 
			Modify(Particle * p, float elapsedTime);

	protected:
		Plane mPlane;
		float mBounce;
	};

}