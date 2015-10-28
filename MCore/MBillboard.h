/*
*	Billboard
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MNode.h"
#include "MRenderObject.h"

namespace Rad {

	class M_ENTRY Billboard : public Node, public RenderObject
	{
		DECLARE_RTTI();

	public:
		Billboard();
		virtual ~Billboard();

		void 
			SetTexture(const String & texName);
		void 
			SetTexture(TexturePtr tex);
		TexturePtr 
			GetTexture();

		void 
			SetSize(const Float2 & size);
		const Float2 & 
			GetSize();

		void 
			SetTexcoord(const RectF & texcoord);
		const RectF & 
			GetTexcoord();

		void 
			SetColor(const Float4 & color);
		const Float4 & 
			GetColor();

		virtual void 
			AddRenderQueue(RenderQueue * RQ);

		virtual void 
			_getWorldPosition(Float3 & pos) { pos = GetWorldPosition(); }
		virtual void 
			_getWorldBound(Aabb & bound) { bound = GetWorldAabb(); }
		virtual void 
			_getWorldTM(Mat4 & form) { form = GetWorldTM(); }

	protected:
		Float2 mSize;
		RectF mTexcoord;
		Float4 mColor;
		bool mNeedUpdate;
	};

}