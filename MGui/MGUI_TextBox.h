/*
*	TextBox
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

	class MGUI_ENTRY TextBox : public Widget
	{
		DECLARE_RTTI();

	public:
		TextBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~TextBox();

		void 
			SetCaption(const UString & caption);
		const UString &
			GetCaption() const;

		void 
			SetFont(Font * font);
		Font * 
			GetFont() const;

		void 
			SetPasswordEnable(bool _enable);
		bool 
			GetPasswordEnable() const;

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

	protected:
		void 
			_updateGlyph();

	protected:
		UString mCaption;
		Font * mFont;

		bool mPasswordEnable;

		Rect mGlyphRect;
		Array<const Glyph *> mGlyphs;
	};

}}
