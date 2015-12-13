#pragma once

#include "MWorldComponent.h"

namespace Rad {

	class M_ENTRY FogController : public WorldComponent
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(WorldComponent);

	public:
		Float2 mRange;
		Float3 mColor;

	public:
		FogController();
		virtual ~FogController();

		virtual int
			Update(float elapsedTime);
	};

}