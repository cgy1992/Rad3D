/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

namespace Exporter {

	struct BlendWeight
	{
		float w[4];

		BlendWeight() { w[0] = 1; w[1] = 0; w[2] = 0; w[3] = 0; }

		void normalize()
		{
			float sum = w[0] + w[1] + w[2] + w[3];

			if (sum > 0)
			{
				w[0] /= sum;
				w[1] /= sum;
				w[2] /= sum;
				w[3] /= sum;
			} 
		}

		bool operator == (const BlendWeight & rk) const
		{
			return w[0] == rk.w[0] && w[1] == rk.w[1] && w[2] == rk.w[2] && w[3] == rk.w[3];
		}
	};

	struct BlendIndex
	{
		unsigned char i[4];

		BlendIndex() { i[0] = 0; i[1] = 0; i[2] = 0; i[3] = 0; }

		bool operator == (const BlendIndex & rk) const
		{
			return i[0] == rk.i[0] && i[1] == rk.i[1] && i[2] == rk.i[2] && i[3] == rk.i[3];
		}
	};

	struct Vertex
	{
		Float3 mPosition;
		Float3 mNormal;
		Float3 mTangent;
		Float4 mColor;
		Float2 mTexcoord;
		Float2 mLightmapUV;
		BlendWeight mBlendWeight;
		BlendIndex mBlendIndex;

		Vertex()
		{
			mPosition = Float3::Zero;
			mNormal = Float3::Zero;
			mTangent = Float3::Zero;
			mColor = Float4(0, 0, 0, 1);
			mTexcoord = Float2::Zero;
			mLightmapUV = Float2::Zero;
		}

		bool operator ==(const Vertex & rk) const
		{
			return 
				mPosition == rk.mPosition &&
				mNormal == rk.mNormal &&
				mTangent == rk.mTangent &&
				mColor == rk.mColor &&
				mTexcoord == rk.mTexcoord &&
				mLightmapUV == rk.mLightmapUV &&
				mBlendWeight == rk.mBlendWeight&&
				mBlendIndex == rk.mBlendIndex;
		}
	};

	class VertexList
	{
	public:
		VertexList();
		~VertexList();

		int 
			Add(const Vertex & v);
		int 
			Size() const;
		const Vertex & 
			GetVertex(int index) const;
		Vertex & 
			GetVertex(int index);

	protected:
		Array<Vertex> mVerts;
	};

}