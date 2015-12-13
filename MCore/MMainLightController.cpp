#include "MMainLightController.h"
#include "MWorld.h"

namespace Rad {

	ImplementRTTI(MainLightController, WorldComponent);

	DF_PROPERTY_BEGIN(MainLightController)
		DF_PROPERTY(MainLightController, mRotation, "Light", "Ambient", PT_Float3)

		DF_PROPERTY_EX(MainLightController, mAmbient, "Light", "Ambient", "PT_Color", PT_Float3)
		DF_PROPERTY_EX(MainLightController, mDiffuse, "Light", "Diffuse", "PT_Color", PT_Float3)
		DF_PROPERTY_EX(MainLightController, mSpecular, "Light", "Specular", "PT_Color", PT_Float3)
	DF_PROPERTY_END()

	MainLightController::MainLightController()
	{
		mName = "MainLight";

		mRotation = Float3(45.0f, -45.0f, 0);
		mAmbient = Float3(0.5f, 0.5f, 0.5f);
		mDiffuse = Float3(1.0f, 1.0f, 1.0f);
		mSpecular = Float3(0.0f, 0.0f, 0.0f);
	}

	MainLightController::~MainLightController()
	{
	}

	int MainLightController::Update(float elapsedTime)
	{
		World::Instance()->MainLight()->SetRotationEx(mRotation);

		World::Instance()->MainLight()->SetAmbient(mAmbient);
		World::Instance()->MainLight()->SetDiffuse(mDiffuse);
		World::Instance()->MainLight()->SetSpecular(mSpecular);

		return 0;
	}

}