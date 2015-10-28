/*
*	Grid
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY GridBox : public Widget
	{
		DECLARE_RTTI();

		enum eAutoResizeType {
			NONE,
			WIDTH,
			HEIGHT,
			ALL,
		};

	public:
		GridBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~GridBox();

		virtual void
			Update(float elapsedTime);

		void 
			SetAutoResizeType(eAutoResizeType type);
		eAutoResizeType 
			GetAutoResizeType();

		void 
			SetGridSize(float w, float h);
		void 
			GetGridSize(float & w, float & h);

		void 
			SetGridAlign(eAlign align);
		eAlign 
			GetGridAlign();

		int 
			GetGridWidgetCount();
		Widget * 
			GetGridWidget(int i);

		void 
			SetColumn(int col);
		int 
			GetColumn();

		void 
			Append(Widget * widget);
		void 
			Remove(Widget * widget);
		void 
			Clear();

	protected:
		int mColumn;
		float mGridWidth, mGridHeight;
		eAutoResizeType mAutoResizeType;
		eAlign mGridAlign;
		Array<Widget *> mGridedWidgets;
	};

}}