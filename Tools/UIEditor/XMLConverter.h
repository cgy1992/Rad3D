/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

namespace Rad {

	struct WidgetNode
	{
		MGUI::Widget * _widget;

		WidgetNode * _next_sibling;
		WidgetNode * _first_child;
		WidgetNode * _last_child;

		WidgetNode()
			: _widget(NULL)
			, _next_sibling(NULL)
			, _first_child(NULL)
			, _last_child(NULL)
		{
		}

		~WidgetNode()
		{
			WidgetNode * e = _first_child;
			while (e != NULL)
			{
				WidgetNode * next = e->_next_sibling;

				delete e;

				e = next;
			}
		}

		void
			WidgetNode::append(WidgetNode * child)
		{
			if (_last_child != 0)
			{
				_last_child->_next_sibling = child;
				_last_child = child;
			}
			else
			{
				_first_child = _last_child = child;
			}
		}
	};

	class XMLConverter
	{
	public:
		XMLConverter();
		~XMLConverter();

		void 
			DoConvertFloder(const String & path, const String & prefixName);

		void 
			ConvertFloder(const String & path);

		void
			SaveBinary(const String & filename);
	protected:
		void 
			Clear();
		void 
			OnWidgetLoaded(MGUI::Widget * w);

	protected:
		WidgetNode * mTree;

	};

}