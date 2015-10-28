#include "MGUI_RenderItem.h"

namespace Rad { namespace MGUI {

	RenderItem::RenderItem(int _order, Texture * _texture)
	{
		mOrder = _order;
		mTexture = _texture;
	}

	RenderItem::~RenderItem()
	{
		mOrder = 0;
		mTexture = NULL;
		mVertices.Clear();
	}

	void RenderItem::AddTri(const Vertex * _tri)
	{
		mVertices.PushBack(_tri[0]);
		mVertices.PushBack(_tri[1]);
		mVertices.PushBack(_tri[2]);
	}

	void RenderItem::AddQuad(const Vertex * _quad)
	{
		mVertices.PushBack(_quad[0]);
		mVertices.PushBack(_quad[1]);
		mVertices.PushBack(_quad[2]);

		mVertices.PushBack(_quad[2]);
		mVertices.PushBack(_quad[1]);
		mVertices.PushBack(_quad[3]);
	}

	void RenderItem::DoRender()
	{
		if (mVertices.Size() == 0)
			return ;

		const Vertex * vb = &mVertices[0];
		int primCount = mVertices.Size() / 3;

		VertexDeclaration vdecl;
		vdecl.AddElement(eVertexSemantic::POSITION, eVertexType::FLOAT2);
		vdecl.AddElement(eVertexSemantic::COLOR, eVertexType::FLOAT4);
		vdecl.AddElement(eVertexSemantic::TEXCOORD0, eVertexType::FLOAT2);

		RenderSystem::Instance()->SetTexture(0, mTexture);
		RenderSystem::Instance()->RenderEx(&vdecl, vb, NULL, ePrimType::TRIANGLE_LIST, primCount);
	}

}}