/*
*	TabPage
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	class TabControl;

	class MGUI_ENTRY TabPage : public Widget
	{
	public:
		TabPage(const LookFeel * _lookfeel, TabControl * _parent);
		virtual ~TabPage();

		virtual void 
			UpdateRenderItem(Layout * layout);

	protected:
		TabControl * mTabControl;
	};

}}

