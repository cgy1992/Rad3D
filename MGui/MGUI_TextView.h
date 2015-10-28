/*
*	TextView
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"
#include "MGUI_Widget.h"
#include "MGUI_FontManager.h"
#include "MGUI_RenderItem.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY TextView : public Widget
	{
		DECLARE_RTTI();

	public:
		TextView(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~TextView();

		void 
			SetCaption(const UString & caption);
		const UString & 
			GetCaption();

		void 
			SetFont(Font * font);
		Font * 
			GetFont();

		void 
			SetVerticalSeparation(float _pixel);
		float 
			GetVerticalSeparation();

		void 
			SetAutoLineFeed(bool _auto);
		bool 
			GetAutoLineFeed();
		
		void 
			SetMaxWidth(float _width);
		float 
			GetMaxWidth();

		int 
			GetGlyphCount();
		const Glyph * 
			GetGlyph(int index);
		const Rect & 
			GetGlyphRect();

		virtual void
			Update(float elapsedTime);
		virtual void 
			UpdateRenderItem(Layout * layout);

		int
			_mapTextIndex(float _x, float _y);
		void
			_updateSelectorRenderItem(RenderItem * ri, const Float4 & color, int selectIndex);
		void
			_updateSelectedRenderItem(RenderItem * ri, const Float4 & color, int startIndex, int endIndex);

		void
			_getLineOffset(int & line, int & offset, int index);
		int
			_getPrevLineIndex(int line, int offset, int index);
		int
			_getNextLineIndex(int line, int offset, int index);

	protected:
		void 
			_updateGlyph();
		void 
			_updateRect();

	protected:
		UString mCaption;

		Font * mFont;

		float mVerticalSeparation;

		bool mAutoLineFeed;
		float mMaxWidth;

		Rect mGlyphRect;
		Array<const Glyph *> mGlyphs;
	};

}}
