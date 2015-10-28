/*
*	MGUI Loader
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Layout.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Loader
	{
	public:
		enum {
			K_Version = 1,
		};

		tEvent1<Widget *> E_WidgetLoaded;

	public:
		Layout * 
			Load(const String & source, Widget * parent);
		Layout * 
			Load(DataStreamPtr stream, Widget * parent);

	protected:
		void 
			_loadWidget(Widget * _widget, xml_node _node);
	};

}}