/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Light.h"
#include "LightFX_Entity.h"

namespace Rad {

	class FX_Rasterizer;

	class FX_Mesh : public FX_Entity
	{
		DECLARE_RTTI();

		friend class FX_Rasterizer;

	public:
		FX_Mesh();
		virtual ~FX_Mesh();

		void SetUserData(void * userData) { mUserData = userData; }
		void * GetUserData() { return mUserData; }

		void SetLightingMode(int mode) { mLightingMode = mode; }
		int GetLightingMode() { return mLightingMode; }

		void SetCastShadow(bool b) { mCastShadow = b; }
		bool IsCastShadow() { return mCastShadow; }

		void SetRecieveShadow(bool b) { mReceiveShadow = b; }
		bool IsRecieveShadow() { return mReceiveShadow; }

		void SetLightingMapSize(int w, int h) { mLightingMapWidth = w; mLightingMapHeight = h; }
		int GetLightingMapWidth() { return mLightingMapWidth; }
		int GetLightingMapHeight() {return mLightingMapHeight; }

		int NumOfVertices() { return mVertexBuffer.Size(); }
		int NumOfTriangles() { return mTriBuffer.Size(); }
		int NumOfMaterial() { return mMtlBuffer.Size(); }

		void Alloc(int numVertex, int numTriangle, int numMaterial);

		void Lock(FX_Vertex ** ppVertex, FX_Triangle ** ppTriangle, FX_Material ** ppMaterial);
		void Unlock();

		void BuildKDTree();
		bool Valid();
		const Aabb & GetBound();

		void RayCheck(FX_Contract & contract, const Ray & ray, float length);

		void CalcuLightingMap();
		void CalcuAmbientOcclusion();
		void CalcuVertexColor();
		void Finish();

		void GetLightingColor(Array<Color> & colors);
		void GetLightingMap(Array<Color> & colors);

	protected:
		bool _addTri(KDTree<int>::Node * node, int triIndex);
		bool _isLightVisible(FX_Light * light);

		void _rayCheckImp(FX_Contract & contract, KDTree<int>::Node * node, const Ray & ray, float length);
		Float4 _getTextureColor(TexturePtr texture, Float2 uv);

		Float3 _doLighting(const FX_Vertex & v, FX_Light * pLight, bool shadow);

	protected:
		Array<FX_Vertex> mVertexBuffer;
		Array<FX_Triangle> mTriBuffer;
		Array<FX_Material> mMtlBuffer;
		KDTree<int> mKDTree;

		void * mUserData;
		int mLightingMode;
		bool mCastShadow;
		bool mReceiveShadow;
		int mLightingMapWidth;
		int mLightingMapHeight;

		Array<Float3> mLightingColor;
		Array<Float3> mLightingMap;
	};

}