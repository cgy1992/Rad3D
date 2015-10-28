/*
*	MGUI RenderLayer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"
#include "MGUI_RenderItem.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY RenderLayer
	{
	public:
		RenderLayer();
		~RenderLayer();

		void 
			Clear();
		RenderItem * 
			RequestRenderItem(int _order, Texture * _skin);
		void 
			DoRender();

	protected:
		RenderItem * mRenderItemLinker;

	};

}}