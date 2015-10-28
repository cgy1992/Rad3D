/*
*	MGUI RenderItem
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY RenderItem
	{
		DECLARE_POOL_ALLOC();
		DECLARE_LINKER(RenderItem);

	public:
		RenderItem(int _order, Texture * _texture);
		~RenderItem();

		int 
			GetOrder() const { return mOrder; }
		Texture * 
			GetTexture() const { return mTexture; }
		const Vertex * 
			GetVertices() const { return &mVertices[0]; }
		int 
			GetVertexCount() const { return mVertices.Size(); }

		void 
			AddTri(const Vertex * _tri);
		void 
			AddQuad(const Vertex * _quad);

		void 
			DoRender();

	protected:
		int mOrder;
		Texture * mTexture;

		Array<Vertex> mVertices;
	};

}}
