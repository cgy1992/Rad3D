#include "MGUI_RenderLayer.h"

namespace Rad { namespace MGUI {

	RenderLayer::RenderLayer()
		: mRenderItemLinker(NULL)
	{
	}

	RenderLayer::~RenderLayer()
	{
		Clear();
	}

	void RenderLayer::Clear()
	{
		while (mRenderItemLinker != NULL)
		{
			RenderItem * p = mRenderItemLinker;

			LINKER_REMOVE(mRenderItemLinker, p);

			delete p;
		}
	}

	RenderItem * RenderLayer::RequestRenderItem(int _order, Texture * _skin)
	{
		RenderItem * p = mRenderItemLinker;
		while (p != NULL)
		{
			if (p->GetOrder() == _order &&
				p->GetTexture() == _skin)
				return p;

			p = LINKER_NEXT(p);
		}

		RenderItem * ri = new RenderItem(_order, _skin);

		p = mRenderItemLinker;
		while (p != NULL)
		{
			if (p->GetOrder() > _order)
			{
				LINKER_INSERT(mRenderItemLinker, p, ri);

				return ri;
			}
			else if (LINKER_NEXT(p) == NULL)
			{
				LINKER_APPEND_TO(mRenderItemLinker, p, ri);

				return ri;
			}

			p = LINKER_NEXT(p);
		}

		mRenderItemLinker = ri;

		return ri;
	}

	void RenderLayer::DoRender()
	{
		RenderItem * p = mRenderItemLinker;

		while (p != NULL)
		{
			p->DoRender();

			p = LINKER_NEXT(p);
		}
	}

}}


