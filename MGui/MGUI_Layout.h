/*
*	MGUI Layout
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_LookFeel.h"
#include "MGUI_RenderLayer.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Layout : public Widget
	{
		DECLARE_RTTI();

	public:
		static Layout * 
			Load(const String & _filename, Widget * _parent);

	public:
		Layout(const LookFeel * _lookfeel, Widget * _parent = NULL);
		virtual ~Layout();

		RenderItem * 
			GetRenderItem(int _order, Texture * _skin);

		void 
			UpdateRenderItem();
		void 
			DoRender();

	protected:
		RenderLayer mRenderLayer;
	};

}}
