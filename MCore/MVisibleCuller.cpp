#include "MVisibleCuller.h"
#include "MWorld.h"

namespace Rad {

	ImplementRTTI(VisibleCuller, IObject);

	VisibleCuller::VisibleCuller()
		: mFrameId(0)
	{
	}

	VisibleCuller::~VisibleCuller()
	{
	}


	ImplementRTTI(VisibleCullerStandard, VisibleCuller);

	VisibleCullerStandard::VisibleCullerStandard()
	{
	}

	VisibleCullerStandard::~VisibleCullerStandard()
	{
	}

	void VisibleCullerStandard::DoCull(int frameId, Camera * camera)
	{
		if (mFrameId == frameId)
			return ;

		mFrameId = frameId;

		mNodeArray.Clear();
		mLightArray.Clear();

		World::Instance()->E_VisibleCullBegin();

		World::Instance()->MainZone()->ImpVisiblityCull(mNodeArray, camera);
		World::Instance()->ImpCullLights(mLightArray, camera);

		World::Instance()->E_VisibleCullEnd();
	}

}

