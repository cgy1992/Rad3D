#include "MKeyController.h"

namespace Rad {

	//
	DF_PROPERTY_BEGIN(KF_Float)
		DF_PROPERTY(KF_Float, data, "", "Data", PT_Float)
	DF_PROPERTY_END()

	//
	DF_PROPERTY_BEGIN(KF_Float2)
		DF_PROPERTY(KF_Float2, data, "", "Data", PT_Float2)
	DF_PROPERTY_END()

	//
	DF_PROPERTY_BEGIN(KF_Float3)
		DF_PROPERTY(KF_Float3, data, "", "Data", PT_Float3)
	DF_PROPERTY_END()

	//
	DF_PROPERTY_BEGIN(KF_Float4)
		DF_PROPERTY(KF_Float4, data, "", "Data", PT_Float4)
	DF_PROPERTY_END()

	//
	DF_PROPERTY_BEGIN(KF_Color3)
		DF_PROPERTY_EX(KF_Color3, data, "", "Data", "PT_Color3", PT_Float3)
	DF_PROPERTY_END()

	//
	DF_PROPERTY_BEGIN(KF_Color4)
		DF_PROPERTY_EX(KF_Color4, data, "", "Data", "PT_Color4", PT_Float4)
	DF_PROPERTY_END()

	//
	DF_PROPERTY_BEGIN(KF_Trasnform)
		DF_PROPERTY(KF_Trasnform, position, "", "Position", PT_Float3)
		DF_PROPERTY(KF_Trasnform, rotation, "", "Rotation", PT_Float4)
		DF_PROPERTY(KF_Trasnform, scale, "", "Scale", PT_Float3)
	DF_PROPERTY_END()
}