#include "LightFX_Light.h"

namespace Rad {

	void FX_DoLighting(float & kd, float & ka, float & ks, const FX_Vertex & v, FX_Light * pLight)
	{
		switch (pLight->Type)
		{
		case eLightType::DIRECTION:
			{
				kd = v.Normal.Dot(-pLight->Direction);

				kd = Math::Clamp(kd, 0.0f, 1.0f);
				ks = 1;
				ka = 1;
			}
			break;

		case eLightType::POINT:
			{
				Float3 lightDir = pLight->Position - v.Position;
				float length = lightDir.Normalize();

				kd = v.Normal.Dot(lightDir);
				ka = (length - pLight->AttenStart) / (pLight->AttenEnd - pLight->AttenStart);

				kd = Math::Clamp(kd, 0.0f, 1.0f);
				ka = Math::Pow(1 - Math::Clamp(ka, 0.0f, 1.0f), pLight->AttenFallOff);
				ks = 1;
			}
			break;

		case eLightType::SPOT:
			{
				Float3 spotDir = pLight->Position - v.Position;
				float length = spotDir.Normalize();

				float kd = v.Normal.Dot(-pLight->Direction);
				float ka = (length - pLight->AttenStart) / (pLight->AttenEnd - pLight->AttenStart);
				float ks = (spotDir.Dot(pLight->Direction) - pLight->SpotOuter) / (pLight->SpotInner - pLight->SpotOuter);

				kd = Math::Clamp(kd, 0.0f, 1.0f);
				ka = Math::Pow(1 - Math::Clamp(ka, 0.0f, 1.0f), pLight->AttenFallOff);
				ks = Math::Pow(Math::Clamp(ks, 0.0f, 1.0f), pLight->SpotFallOff);
			}
			break;

		default:
			d_assert(0);
			break;
		}
	}

}