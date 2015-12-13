#include "MGrassController.h"
#include "MGrassManager.h"

namespace Rad {

	ImplementRTTI(GrassController, WorldComponent);

	DF_PROPERTY_BEGIN(GrassController)
		DF_PROPERTY(GrassController, mWaveDir, "Grass", "WaveDir", PT_Float2)
		DF_PROPERTY(GrassController, mWaveSpeed, "Grass", "WaveSpeed", PT_Float)
		DF_PROPERTY(GrassController, mWaveStrength, "Grass", "WaveStrength", PT_Float)
		DF_PROPERTY(GrassController, mVisibleRadius, "Grass", "VisibleRadius", PT_Float)
	DF_PROPERTY_END()

	GrassController::GrassController()
	{
		mName = "Grass";

		mWaveDir = Float2(1, 0);
		mWaveSpeed = 0.8f;
		mWaveStrength = 0.15f;
		mVisibleRadius = 25 * METER_LEN;
	}

	GrassController::~GrassController()
	{
	}

	int GrassController::Update(float elapsedTime)
	{
		GrassManager::Instance()->SetWaveParam(mWaveDir, mWaveSpeed, mWaveStrength);
		GrassManager::Instance()->SetVisibleRadius(mVisibleRadius);

		return 0;
	}

}