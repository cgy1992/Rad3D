#include "LightFX_Mesh.h"
#include "LightFX_World.h"
#include "LightFX_Rasterizer.h"
#include "MUtil.h"

namespace Rad {

	ImplementRTTI(FX_Mesh, FX_Entity);

	FX_Mesh::FX_Mesh()
	{
		mUserData = NULL;
		mLightingMode = eStaticLightingMode::NONE;
		mCastShadow = true;
		mReceiveShadow = true;

		mLightingMapWidth = 128;
		mLightingMapHeight = 128;
	}

	FX_Mesh::~FX_Mesh()
	{
		mVertexBuffer.Clear();
		mTriBuffer.Clear();
		mMtlBuffer.Clear();
		mKDTree.Clear();
	}

	void FX_Mesh::Alloc(int numVertex, int numTriangle, int numMaterial)
	{
		if (numVertex >= 0) {
			mVertexBuffer.Resize(numVertex);
		}
		if (numTriangle >= 0) {
			mTriBuffer.Resize(numTriangle);
		}
		if (numMaterial > 0) {
			mMtlBuffer.Resize(numMaterial);
		}
	}

	void FX_Mesh::Lock(FX_Vertex ** ppVertex, FX_Triangle ** ppTriangle, FX_Material ** ppMaterial)
	{
		*ppVertex = &mVertexBuffer[0];
		*ppTriangle = &mTriBuffer[0];
		*ppMaterial = &mMtlBuffer[0];
	}

	void FX_Mesh::Unlock()
	{
	}

	void FX_Mesh::BuildKDTree()
	{
		if (mVertexBuffer.Size() == 0)
			return ;

		Aabb bound;
		bound.minimum = mVertexBuffer[0].Position;
		bound.maximum = mVertexBuffer[0].Position;

		for (int i = 1; i < mVertexBuffer.Size(); ++i)
		{
			bound.minimum = Float3::Minimum(bound.minimum, mVertexBuffer[i].Position);
			bound.maximum = Float3::Maximum(bound.maximum, mVertexBuffer[i].Position);
		}

		mKDTree.Build(bound, 4);

		for (int i = 0; i < mTriBuffer.Size(); ++i)
		{
			bool hr = _addTri(mKDTree.GetRoot(), i);
			d_assert (hr != NULL);
		}
	}

	bool FX_Mesh::Valid()
	{
		return mKDTree.GetRoot() != NULL;
	}

	const Aabb & FX_Mesh::GetBound()
	{
		d_assert (Valid());

		return mKDTree.GetRoot()->aabb;
	}

	Float4 FX_Mesh::_getTextureColor(TexturePtr texture, Float2 uv)
	{
		while (uv.x < 0)
			uv.x += 1;

		while (uv.x > 1)
			uv.x -= 1;

		while (uv.y < 0)
			uv.y += 1;

		while (uv.y > 1)
			uv.y -= 1;

		float fu = uv.x * (texture->GetWidth() - 1);
		float fv = uv.y * (texture->GetHeight() - 1);

		int iu0 = (int)fu;
		int iv0 = (int)fv;
		int iu1 = iu0 + 1;
		int iv1 = iv0 + 1;

		if (iu1 > texture->GetWidth() - 1)
			iu1 = iu0;

		if (iv1 > texture->GetHeight() - 1)
			iv1 = iv0;

		Float4 c0, c1, c2, c3;

		if (!texture->GetColorData(c0, iu0, iv0))
			return Float4(1, 1, 1, 1);
		if (!texture->GetColorData(c1, iu1, iv0))
			return Float4(1, 1, 1, 1);
		if (!texture->GetColorData(c2, iu0, iv1))
			return Float4(1, 1, 1, 1);
		if (!texture->GetColorData(c3, iu1, iv1))
			return Float4(1, 1, 1, 1);

		float du = fu - iu0;
		float dv = fv - iv0;

		Float4 c4 = c0 + (c1 - c0) * du;
		Float4 c5 = c2 + (c3 - c2) * du;

		return c4 + (c5 - c4) * dv;
	}

	void FX_Mesh::_rayCheckImp(FX_Contract & contract, KDTree<int>::Node * node, const Ray & ray, float length)
	{
		float dist = 0;

		if (!ray.Intersect(&dist, node->aabb) || dist >= contract.td || dist >= length)
			return ;

		for (int i = 0; i < node->elems.Size(); ++i)
		{
			int triIndex = node->elems[i];
			const FX_Triangle & triangle = mTriBuffer[triIndex];

			const Float3 & a = mVertexBuffer[triangle.Index0].Position;
			const Float3 & b = mVertexBuffer[triangle.Index1].Position;
			const Float3 & c = mVertexBuffer[triangle.Index2].Position;

			const FX_Material & m = mMtlBuffer[triangle.MaterialId];

			float tu, tv;
			if (ray.Intersect(&dist, tu, tv, a, b, c) && dist < contract.td && dist <= length)
			{
				if (m.texture != NULL)
				{
					const Float2 & uv0 = mVertexBuffer[triangle.Index0].UV;
					const Float2 & uv1 = mVertexBuffer[triangle.Index1].UV;
					const Float2 & uv2 = mVertexBuffer[triangle.Index2].UV;

					Float2 uv = uv0 * (1 - tu - tv) + uv1 * tu + uv2 * tv;

					Float4 color = _getTextureColor(m.texture, uv);

					if (color.a < 0.5f)
						continue ;
				}

				contract.td = dist;
				contract.triIndex = triIndex;
				contract.tu = tu;
				contract.tv = tv;
				contract.tn = Float3::GetNormal(a, b, c);
				contract.entity = this;
			}
		}

		if (node->child[0] != NULL)
			_rayCheckImp(contract, node->child[0], ray, length);

		if (node->child[1] != NULL)
			_rayCheckImp(contract, node->child[1], ray, length);
	}

	void FX_Mesh::RayCheck(FX_Contract & contract, const Ray & ray, float length)
	{
		d_assert (Valid());

		if (mCastShadow)
		{
			_rayCheckImp(contract, mKDTree.GetRoot(), ray, length);
		}
	}

	bool FX_Mesh::_addTri(KDTree<int>::Node * node, int t)
	{
		const FX_Triangle & triangle = mTriBuffer[t];

		const Float3 & a = mVertexBuffer[triangle.Index0].Position;
		const Float3 & b = mVertexBuffer[triangle.Index1].Position;
		const Float3 & c = mVertexBuffer[triangle.Index2].Position;

		if (node->aabb.Contain(a) && node->aabb.Contain(b) && node->aabb.Contain(c))
		{
			if (node->child[0] != NULL && _addTri(node->child[0], t))
				return true;

			if (node->child[1] != NULL && _addTri(node->child[1], t))
				return true;

			node->elems.PushBack(t);

			return true;
		}

		return false;
	}

	bool FX_Mesh::_isLightVisible(FX_Light * pLight)
	{
		if (pLight->Type == eLightType::POINT || pLight->Type == eLightType::SPOT)
		{
			Float3 lightExtend = Float3(pLight->AttenEnd, pLight->AttenEnd, pLight->AttenEnd);

			Aabb lightBound;
			lightBound.minimum = pLight->Position - lightExtend;
			lightBound.maximum = pLight->Position + lightExtend;

			return lightBound.Intersect(mKDTree.GetRoot()->aabb);
		}

		return true;
	}

	void FX_Mesh::CalcuLightingMap()
	{
		d_assert (mLightingMode == eStaticLightingMode::LIGHTING_MAP);

		int msaa = FX_World::Instance()->GetSetting()->MSAA;
		int width = msaa * mLightingMapWidth;
		int height = msaa * mLightingMapHeight;

		FX_Rasterizer r(this, width, height, false);
		for (int i = 0; i < mTriBuffer.Size(); ++i)
		{
			const FX_Triangle & tri = mTriBuffer[i];

			r.DoRasterize2(mVertexBuffer[tri.Index0], mVertexBuffer[tri.Index1], mVertexBuffer[tri.Index2]);
		}

		mLightingMap.Resize(mLightingMapWidth * mLightingMapHeight);

		for (int j = 0; j < mLightingMapHeight; ++j)
		{
			for (int i = 0; i < mLightingMapWidth; ++i)
			{
				Float3 color = Float3(0, 0, 0);

				for (int n = 0; n < msaa; ++n)
				{
					for (int m = 0; m < msaa; ++m)
					{
						int x = i * msaa + m;
						int y = j * msaa + n;

						color += r._rmap[y * width + x];
					}
				}

				color /= (float)msaa * msaa;

				mLightingMap[j * mLightingMapWidth + i] = color + FX_World::Instance()->GetSetting()->Ambient;
			}
		}
	}

	void FX_Mesh::CalcuAmbientOcclusion()
	{
		int msaa = FX_World::Instance()->GetSetting()->MSAA;
		int width = msaa * mLightingMapWidth;
		int height = msaa * mLightingMapHeight;

		FX_Rasterizer r(this, width, height, true);
		for (int i = 0; i < mTriBuffer.Size(); ++i)
		{
			const FX_Triangle & tri = mTriBuffer[i];

			r.DoRasterize2(mVertexBuffer[tri.Index0], mVertexBuffer[tri.Index1], mVertexBuffer[tri.Index2]);
		}

		for (int j = 0; j < mLightingMapHeight; ++j)
		{
			for (int i = 0; i < mLightingMapWidth; ++i)
			{
				Float3 color = Float3(0, 0, 0);

				for (int n = 0; n < msaa; ++n)
				{
					for (int m = 0; m < msaa; ++m)
					{
						int x = i * msaa + m;
						int y = j * msaa + n;

						color += r._rmap[y * width + x];
					}
				}

				color /= (float)msaa * msaa;
				color.Saturate();

				mLightingMap[j * mLightingMapWidth + i] *= color;
			}
		}
	}

	void FX_Mesh::CalcuVertexColor()
	{
		d_assert (mLightingMode == eStaticLightingMode::LIGHTING_COLOR);

		mLightingColor.Resize(mVertexBuffer.Size());

		for (int i = 0; i < mVertexBuffer.Size(); ++i)
		{
			mLightingColor[i] = FX_World::Instance()->GetSetting()->Ambient;
		}

		for (int i = 0; i < FX_World::Instance()->GetLightCount(); ++i)
		{
			FX_Light * pLight = FX_World::Instance()->GetLight(i);

			if (_isLightVisible(pLight))
			{
				for (int i = 0; i < mVertexBuffer.Size(); ++i)
				{
					mLightingColor[i] += _doLighting(mVertexBuffer[i], pLight, mReceiveShadow);
				}
			}
		}
	}

	Float3 FX_Mesh::_doLighting(const FX_Vertex & v, FX_Light * pLight, bool shadow)
	{
		float kd = 0, ka = 0, ks = 0;

		FX_DoLighting(kd, ka, ks, v, pLight);

		if (kd * ka * ks >= 0 && shadow)
		{
			float len = 0;
			Ray ray;
			
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

			ray.orig = v.Position + ray.dir * UNIT_METRES * 0.01f;

			if (len > 0.01f * UNIT_METRES)
			{
				FX_Contract contract;
				if (FX_World::Instance()->RayCheck(contract, ray, len, FX_Flag::MESH))
				{
					kd = ka = ks = 0;
				}
			}
		}

		return kd * ka * ks * mMtlBuffer[v.MaterialId].color * pLight->Color;
	}

	void FX_Mesh::GetLightingColor(Array<Rgba32> & colors)
	{
		d_assert (mLightingMode == eStaticLightingMode::LIGHTING_COLOR);

		colors.Resize(mLightingColor.Size());

		for (int i = 0; i < colors.Size(); ++i)
		{
			colors[i] = util::TO_RGBE(mLightingColor[i]);
		}
	}

	void FX_Mesh::GetLightingMap(Array<Rgba32> & colors)
	{
		d_assert (mLightingMode == eStaticLightingMode::LIGHTING_MAP);

		colors.Resize(mLightingMap.Size());

		for (int i = 0; i < colors.Size(); ++i)
		{
			colors[i] = util::TO_RGBE(mLightingMap[i]);
			/*colors[i].value = M_RGBA_F(
				mLightingMap[i].r,
				mLightingMap[i].g,
				mLightingMap[i].b,
				1);*/
		}
	}

	void FX_Mesh::Finish()
	{
		if (mLightingMode == eStaticLightingMode::LIGHTING_MAP)
		{
			int xMapSize = mLightingMapWidth;
			int zMapSize = mLightingMapHeight;

			FX_Rasterizer::DoBlur(&mLightingMap[0], xMapSize, zMapSize);
		}
	}
}

