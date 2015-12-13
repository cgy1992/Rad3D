#pragma once

#include "MWorldComponent.h"

namespace Rad {

	class M_ENTRY MainLightController : public WorldComponent
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(WorldComponent);

	public:
		Float3 mRotation;
		Float3 mAmbient;
		Float3 mDiffuse;
		Float3 mSpecular;

	public:
		MainLightController();
		virtual ~MainLightController();

		virtual int
			Update(float elapsedTime);
	};

}