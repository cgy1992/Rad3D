#include "LightFX_Grass.h"
#include "LightFX_World.h"

namespace Rad {

	void FX_GrassBlock::GetGrass(Array<FX_Grass *> & ga, const Float3 & p, float radius)
	{
		RectF rect;
		rect.x1 = p.x - radius * 0.5f;
		rect.y1 = p.z - radius * 0.5f;
		rect.x2 = p.x + radius * 0.5f;
		rect.y2 = p.z + radius * 0.5f;

		if (!mRect.Intersect(rect))
			return ;

		float radiusSq = radius * radius;
		for (int i = 0; i < mGrassArray.Size(); ++i)
		{
			FX_Grass * gs = &mGrassArray[i];
			if (gs->mGI->Position.DistanceSq(p) < radiusSq)
			{
				ga.PushBack(gs);
			}
		}
	}

	bool FX_GrassBlock::IsVisible(FX_Light * pLight)
	{
		if (pLight->Type == eLightType::POINT || pLight->Type == eLightType::SPOT)
		{
			Float3 lightExtend = Float3(pLight->AttenEnd, pLight->AttenEnd, pLight->AttenEnd);

			Aabb lightBound;
			lightBound.minimum = pLight->Position - lightExtend;
			lightBound.maximum = pLight->Position + lightExtend;

			RectF rect;
			rect.x1 = lightBound.minimum.x - UNIT_METRES;
			rect.y1 = lightBound.minimum.z - UNIT_METRES;
			rect.x2 = lightBound.maximum.x + UNIT_METRES;
			rect.y2 = lightBound.maximum.z + UNIT_METRES;

			return mRect.Intersect(rect);
		}

		return true;
	}

	//
	//
	void FX_GrassBlock::DoAmbient()
	{
		for (int i = 0; i < mGrassArray.Size(); ++i)
		{
			FX_Grass & gs = mGrassArray[i];

			for (int j = 0; j < array_count(gs.mVertex); ++j)
			{
				gs.mLightingColor[j] += FX_World::Instance()->GetSetting()->Ambient;
			}
		}
	}

	void FX_GrassBlock::DoLighting(FX_Light * pLight)
	{
		for (int i = 0; i < mGrassArray.Size(); ++i)
		{
			FX_Grass & gs = mGrassArray[i];

			for (int j = 0; j < array_count(gs.mVertex); ++j)
			{
				FX_Vertex v = gs.mVertex[j];

				float kd = 0, ka = 0, ks = 0;

				FX_DoLighting(kd, ka, ks, v, pLight);

				if (kd * ka * ks >= 0)
				{
					float len = 0;
					Ray ray;

					ray.orig = v.Position + v.Normal * UNIT_METRES * 0.01f;

					if (pLight->Type != eLightType::DIRECTION)
					{
						ray.dir = pLight->Position - v.Position;
						len = ray.dir.Normalize();
					}
					else
					{
						ray.dir = -pLight->Direction;
						len = MAX_FLOAT;
					}

					if (len > 0.01f * UNIT_METRES)
					{
						FX_Contract contract;
						if (FX_World::Instance()->RayCheck(contract, ray, len, FX_Flag::MESH))
						{
							kd = ka = ks = 0;
						}
					}
				}

				gs.mLightingColor[j] += kd * ka * ks * pLight->Color;
			}
		}
	}

	void FX_GrassBlock::DoAmbientOcclusion()
	{
		for (int i = 0; i < mGrassArray.Size(); ++i)
		{
			FX_Grass & gs = mGrassArray[i];

			for (int j = 0; j < array_count(gs.mVertex); ++j)
			{
				FX_Vertex v = gs.mVertex[j];

				Float3 ao = FX_AOGenerator::Instance()->DoAmbientOcclusion(v, FX_Flag::MESH, NULL);

				gs.mLightingColor[j] *= ao;
			}
		}
	}



	//
	//
	FX_GrassManager::FX_GrassManager()
	{
	}

	FX_GrassManager::~FX_GrassManager()
	{
		Clear();
	}

	void FX_GrassManager::Clear()
	{
		for (int i = 0; i < mGrassBlockArray.Size(); ++i)
		{
			delete mGrassBlockArray[i];
		}
		mGrassBlockArray.Clear();
	}

	void FX_GrassManager::Build(int xblock, int zblock, float xblockSize, float zblockSize)
	{
		d_assert (mGrassBlockArray.Empty() && xblock > 0 && zblock > 0);

		for (int j = 0; j < zblock; ++j)
		{
			for (int i = 0; i < xblock; ++i)
			{
				FX_GrassBlock * gblock = new FX_GrassBlock;

				gblock->mRect.x1 = xblockSize * (i + 0);
				gblock->mRect.y1 = zblockSize * (j + 0);
				gblock->mRect.x2 = xblockSize * (i + 1);
				gblock->mRect.y2 = zblockSize * (j + 1);

				mGrassBlockArray.PushBack(gblock);
			}
		}
	}

	void FX_GrassManager::AddGrass(const FX_Grass & gs)
	{
		for (int i = 0; i < mGrassBlockArray.Size(); ++i)
		{
			FX_GrassBlock * gblock = mGrassBlockArray[i];

			if (gs.mGI->Position.x >= gblock->mRect.x1 && gs.mGI->Position.x <= gblock->mRect.x2 &&
				gs.mGI->Position.z >= gblock->mRect.y1 && gs.mGI->Position.z <= gblock->mRect.y2)
			{
				gblock->mGrassArray.PushBack(gs);
				return ;
			}
		}
	}

	void FX_GrassManager::GetGrass(Array<FX_Grass *> & ga, const Float3 & p, float radius)
	{
		for (int i = 0; i < mGrassBlockArray.Size(); ++i)
		{
			FX_GrassBlock * gblock = mGrassBlockArray[i];

			gblock->GetGrass(ga, p, radius);
		}
	}

	Float3 FX_GrassManager::DoAmbientOcclusion(const FX_Vertex & v)
	{
		float radius = FX_World::Instance()->GetSetting()->AOGrassRadius;
		float factory = FX_World::Instance()->GetSetting()->AOGrassFactory;

		if (radius <= 0 || factory <= 0)
			return Float3(1, 1, 1);

		Float3 color = Float3(1, 1, 1);
		float i_radius = 1 / radius;
		float i_factory = 1 / factory;

		float ao = 0;
		Array<FX_Grass *> garray;
		GetGrass(garray, v.Position, radius);

		for (int i = 0; i < garray.Size(); ++i)
		{
			const FX_Grass * gs = garray[i];

			float ks = Min(1.0f, gs->mGI->Size * i_factory);
			float ka = Max(0.0f, gs->mGI->Normal.Dot(v.Normal));
			float kd = 1 - Math::Clamp(gs->mGI->Position.Distance(v.Position) * i_radius, 0.0f, 1.0f);

			ao += ks * ka * kd;
		}

		ao = Math::Clamp(ao, 0.0f, 1.0f);

		return Float3::Lerp(FX_World::Instance()->GetSetting()->AOColor, Float3(1, 1, 1), 1 - ao);
	}

	void FX_GrassManager::CalcuVertexColor()
	{
		for (int i = 0; i < mGrassBlockArray.Size(); ++i)
		{
			FX_GrassBlock * pBlock = mGrassBlockArray[i];

			pBlock->DoAmbient();

			for (int l = 0; l < FX_World::Instance()->GetLightCount(); ++l)
			{
				FX_Light * pLight = FX_World::Instance()->GetLight(l);

				if (pBlock->IsVisible(pLight))
				{
					pBlock->DoLighting(pLight);
				}
			}

			FX_World::Instance()->SetProgress((int)(i / (float)mGrassBlockArray.Size() * 100.0f));
		}
	}

	void FX_GrassManager::CalcuAmbientOcclusion()
	{
		for (int i = 0; i < mGrassBlockArray.Size(); ++i)
		{
			FX_GrassBlock * pBlock = mGrassBlockArray[i];

			pBlock->DoAmbientOcclusion();

			FX_World::Instance()->SetProgress((int)(i / (float)mGrassBlockArray.Size() * 100.0f));
		}
	}

	void FX_GrassManager::ProcessResult()
	{
		for (int i = 0; i < mGrassBlockArray.Size(); ++i)
		{
			FX_GrassBlock * pBlock = mGrassBlockArray[i];

			for (int j = 0; j < pBlock->mGrassArray.Size(); ++j)
			{
				FX_Grass & gs = pBlock->mGrassArray[j];

				Float3 sum = Float3(0, 0, 0);
				for (int k = 0; k < array_count(gs.mLightingColor); ++k)
				{
					sum += gs.mLightingColor[k];
				}

				gs.mGI->LightingColor = sum / FX_Grass::K_SEGMENT;
			}
		}
	}
}