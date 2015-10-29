/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "LightFX_Define.h"
#include "LightFX_Entity.h"
#include "LightFX_Light.h"
#include "MTerrain.h"

namespace Rad {

	class FX_Terrain : public FX_Entity
	{
		DECLARE_RTTI();

	public:
		FX_Terrain(Terrain * pTerrain);
		virtual ~FX_Terrain();

		void SetMaterialColor(const Float3 & color);

		void BuildKDTree();

		void RayCheck(FX_Contract & contract, const Ray & ray, float length);

		void CalcuLightingMap();
		void CalcuAmbientOcclusion();
		void Finish();

		void GetLightingMap(Array<Color> & colors);

	protected:
		bool _addTri(KDTree<int>::Node * node, int triIndex);
		void _rayCheckImp(FX_Contract & contract, KDTree<int>::Node * node, const Ray & ray, float length);

		Float3 _doLighting(const FX_Vertex & v, FX_Light * pLight);

	protected:
		Terrain * mTerrain;
		Array<FX_Vertex> mVertexBuffer;
		Array<FX_Triangle> mTriBuffer;
		Array<Float3> mLightingMap;
		KDTree<int> mKDTree;

		Float3 mMaterialColor;
	};

}