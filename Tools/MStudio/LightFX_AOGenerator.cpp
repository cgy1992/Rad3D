#include "LightFX_AOGenerator.h"
#include "LightFX_World.h"

namespace Rad {

	ImplementSingleton(FX_AOGenerator);

	FX_AOGenerator::FX_AOGenerator()
	{
		int index = 0;

		for (int i = 0; i < 16; ++i)
		{
			hd[index].y = 0;
			hd[index].x = cos(i / 16.0f * PI2);
			hd[index].z = sin(i / 16.0f * PI2);
			hd[index].w = 1.0f;
			++index;
		}

		for (int i = 0; i < 8; ++i)
		{
			float r = sin(PI / 6.0f);
			hd[index].y = cos(PI / 6.0f);
			hd[index].x = cos(i / 8.0f * PI2) * r;
			hd[index].z = sin(i / 8.0f * PI2) * r;
			hd[index].w = 1.0f;
			++index;
		}

		for (int i = 0; i < 4; ++i)
		{
			float r = sin(PI / 3.0f);
			hd[index].y = cos(PI / 3.0f);
			hd[index].x = cos(i / 4.0f * PI2) * r;
			hd[index].z = sin(i / 4.0f * PI2) * r;
			hd[index].w = 1.0f;
			++index;
		}

		hd[index] = Float4(0, 1, 0, 1);
		
		//
		index = 0;

		for (int i = 0; i < 8; ++i)
		{
			ld[index].y = 0;
			ld[index].x = cos(i / 8.0f * PI2);
			ld[index].z = sin(i / 8.0f * PI2);
			ld[index].w = 1.0f;
			++index;
		}

		for (int i = 0; i < 4; ++i)
		{
			float r = sin(0.25f * PI2);
			ld[index].y = cos(0.25f * PI2);
			ld[index].x = cos(i / 4.0f * PI2) * r;
			ld[index].z = sin(i / 4.0f * PI2) * r;
			ld[index].w = 1.0f;
			++index;
		}

		ld[index] = Float4(0, 1, 0, 1);
	}

	FX_AOGenerator::~FX_AOGenerator()
	{
	}

	Float3 FX_AOGenerator::DoAmbientOcclusion(const FX_Vertex & v, int flags, void * entity)
	{
		float strength = FX_World::Instance()->GetSetting()->AOStrength;
		float radius = FX_World::Instance()->GetSetting()->AORadius;
		if (radius <= 0)
			return Float3(1, 1, 1);

		Float3 xaxis = Float3(1, 0, 0);
		Float3 zaxis = Float3(0, 0, 1);
		if (v.Normal.Dot(xaxis) > 0.99f)
		{
			xaxis = Float3(0, -1, 0);
		}

		zaxis = Float3::Cross(xaxis, v.Normal);
		xaxis = Float3::Cross(v.Normal, zaxis);

		Mat4 form; 
		form.MakeRotationAxis(xaxis, v.Normal, zaxis);

		float ao = 0;
		float i_radius = 1 / radius;

		if (FX_World::Instance()->GetSetting()->AOLevel == FX_AOLevel::HIGH)
		{
			for (int i = 0; i < hd.Size(); ++i)
			{
				Ray ray;
				ray.orig = v.Position + v.Normal * 0.01f * METER_LEN;
				ray.dir = Float3(hd[i].x, hd[i].y, hd[i].z);
				ray.dir.TransformN(form);

				FX_Contract contract;
				if (FX_World::Instance()->RayCheck(contract, ray, radius, flags) && contract.entity != entity)
				{
					float ka = Math::Clamp(contract.tn.Dot(-v.Normal), -1.0f, 1.0f);
					ka = Math::RadianToDegree(Math::FastACos(ka));
					ka = 1 - Min(1.0f, ka / 120.0f);

					float kd = contract.td * i_radius;
					kd = 1 - Math::Clamp(kd, 0.0f, 1.0f);

					ao += ka * kd * hd[i].w;
				}
			}

			ao = ao * strength / hd.Size();
		}
		else
		{
			for (int i = 0; i < ld.Size(); ++i)
			{
				Ray ray;
				ray.orig = v.Position + v.Normal * 0.01f * METER_LEN;
				ray.dir = Float3(ld[i].x, ld[i].y, ld[i].z);
				ray.dir.TransformN(form);

				FX_Contract contract;
				if (FX_World::Instance()->RayCheck(contract, ray, radius, flags) && contract.entity != entity)
				{
					float ka = Math::Clamp(contract.tn.Dot(-v.Normal), -1.0f, 1.0f);
					ka = Math::RadianToDegree(Math::FastACos(ka));
					ka = 1 - Min(1.0f, ka / 120.0f);

					float kd = contract.td * i_radius;
					kd = 1 - Math::Clamp(kd, 0.0f, 1.0f);

					ao += ka * kd * ld[i].w;
				}
			}

			ao = ao * strength / ld.Size();
		}

		ao = Math::Clamp(ao, 0.0f, 1.0f);

		return Float3::Lerp(FX_World::Instance()->GetSetting()->AOColor, Float3(1, 1, 1), 1 - ao);
	}

}
