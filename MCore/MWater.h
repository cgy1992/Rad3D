/*
*	Water
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MColMesh.h"
#include "MWaterShader.h"

namespace Rad {

	class M_ENTRY Water : public Node, public RenderObject
	{
		DECLARE_RTTI();
		DECLARE_PROPERTY(Node);

	public:
		typedef float (*WaterDepthFunction)(float x, float z, Water * pWater);
		
		static WaterDepthFunction d_functoin;

	protected:
		FixedString32 mShaderClass;

	public:
		Water();
		virtual ~Water();

		virtual void
			OnPropertyChanged(const Property * p);

		void 
			Build(unsigned char * grid, float gridSize, int w, int h);
		void 
			Destroy();

		virtual void 
			_getWorldPosition(Float3 & pos) { pos = GetWorldPosition(); }
		virtual void 
			_getWorldBound(Aabb & bound) { bound = GetWorldAabb(); }
		virtual void 
			_getWorldTM(Mat4 & form) { form = GetWorldTM(); }
		virtual int
			SortSpecial(RenderObject * obj2, const Float3 & eye);
		
		virtual void
			Update(float elapsedTime);
		virtual void 
			AddRenderQueue(RenderQueue * rq);

		void
			SetShaderClass(const FixedString32 & name);
		const FixedString32 &
			GetShaderClass() { return mShaderClass; }
		void
			SetWaterShader(WaterShaderPtr shader);
		WaterShaderPtr
			GetWaterShader() { return mWaterShader; }

		float 
			GetGridSize() { return mGridSize; }
		int 
			GetGridWidth() { return mGridWidth; }
		int 
			GetGridHeight() { return mGridHeight; }
		unsigned char 
			GetGridInfo(int x, int z) { d_assert(x < mGridWidth && z < mGridHeight); return mGridInfo[z * mGridWidth + x]; }
		const unsigned char * 
			GetGridInfo() { return mGridInfo; }

		float 
			GetXSize() { return mXSize; }
		float 
			GetZSize() { return mZSize; }
		float 
			GetInvGridSize() { return mInvGridSize; }

		ColMesh * 
			GetColMesh() { return &mColMesh; }

		void 
			Optimize();

		bool 
			MapCoord(int & x, int & z, float fx, float fz);

		virtual void 
			Serialize(Serializer & sl);

		virtual void 
			Load(float priority) {}
		virtual void 
			Unload() {}
		virtual void 
			Reload() {}

	protected:
		float mGridSize;
		int mGridWidth, mGridHeight;
		unsigned char * mGridInfo;

		float mXSize, mZSize;
		float mInvGridSize;

		ColMesh mColMesh;

		WaterShaderPtr mWaterShader;
	};

};