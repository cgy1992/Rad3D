#include "MFogController.h"
#include "MRenderRegister.h"

namespace Rad {

	ImplementRTTI(FogController, WorldComponent);

	DF_PROPERTY_BEGIN(FogController)
		DF_PROPERTY(FogController, mRange, "Fog", "Range", PT_Float2)
		DF_PROPERTY_EX(FogController, mColor, "Fog", "Color", "PT_Color", PT_Float3)
	DF_PROPERTY_END()

	FogController::FogController()
		: mRange(20 * METER_LEN, 100 * METER_LEN)
		, mColor(0.5f, 0.5f, 0.5f)
	{
		mName = "Fog";
	}

	FogController::~FogController()
	{
	}

	int FogController::Update(float elapsedTime)
	{
		RenderRegister::Instance()->SetFogParam(mRange.x, mRange.y);
		RenderRegister::Instance()->SetFogColor(mColor);

		return 0;
	}

}