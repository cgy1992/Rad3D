#pragma once

#include "MWorldComponent.h"

namespace Rad {

	class M_ENTRY GrassController : public WorldComponent
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(WorldComponent);

	public:
		Float2 mWaveDir;
		float mWaveSpeed;
		float mWaveStrength;
		float mVisibleRadius;

	public:
		GrassController();
		virtual ~GrassController();

		virtual int
			Update(float elapsedTime);
	};

}