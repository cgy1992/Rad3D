#include "MGUI_TextView.h"
#include "MGUI_Layout.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(TextView, Widget);

	TextView::TextView(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
		, mVerticalSeparation(0)
		, mAutoLineFeed(false)
		, mMaxWidth(128)
	{
		SetPickFlag(ePickFlag::NONE);
		SetInheritColor(false);

		mFont = FontManager::Instance()->GetDefaultFont();
		if (mFont != NULL)
		{
			mGlyphRect.h = mFont->GetHeight();
			mRect.h = mFont->GetHeight();
		}

		SetOrder(eOrder::TEXT);
	}

	TextView::~TextView()
	{
	}

	void TextView::SetCaption(const UString & caption)
	{
		mCaption = caption;

		_updateGlyph();
	}

	const UString & TextView::GetCaption()
	{
		return mCaption;
	}

	void TextView::SetFont(Font * _font)
	{
		mFont = _font;

		_updateGlyph();
	}

	Font * TextView::GetFont()
	{
		return mFont;
	}

	void TextView::SetVerticalSeparation(float _pixel)
	{
		mVerticalSeparation = _pixel;
	}

	float TextView::GetVerticalSeparation()
	{
		return mVerticalSeparation;
	}

	void TextView::SetAutoLineFeed(bool _auto)
	{
		mAutoLineFeed = _auto;
	}

	bool TextView::GetAutoLineFeed()
	{
		return mAutoLineFeed;
	}

	void TextView::SetMaxWidth(float _width)
	{
		mMaxWidth = _width;
	}

	float TextView::GetMaxWidth()
	{
		return mMaxWidth;
	}

	int TextView::GetGlyphCount()
	{
		return mGlyphs.Size();
	}

	const Glyph * TextView::GetGlyph(int index)
	{
		return mGlyphs[index];
	}

	void TextView::Update(float elapsedTime)
	{
		if (mVisible)
		{
			_updateTM();

			_updateRect();
		}
		
		Widget::Update(elapsedTime);
	}

	const Rect & TextView::GetGlyphRect()
	{
		return mGlyphRect;
	}

	void TextView::UpdateRenderItem(Layout * _layout)
	{
		if (mFont == NULL)
			return ;

		if (mAutoLineFeed && mMaxWidth < mFont->GetHeight())
			return ;

		Rect clipRect = mParent->GetClipRect();
		Float4 color = mAbsColor;
		int order = mAbsOrder;
		float scale = mAbsScale;
		float x_base = 0;
		float y_base = 0;
		float x_max = mMaxWidth;

		if (mLookFeel != NULL)
			color *= mLookFeel->GetTextColor(mState);

		Texture * skin = NULL;
		RenderItem * ri = NULL;
		Rect myRect, uvRect;

		for (int i = 0; i < mGlyphs.Size(); ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				x_base = 0;
				y_base += (mFont->GetHeight() + mVerticalSeparation) * scale;
				continue;
			}

			if (mAutoLineFeed && x_base + glyph->advance * scale > x_max)
			{
				x_base = 0;
				y_base += (mFont->GetHeight() + mVerticalSeparation) * scale;
			}

			if (glyph->texture != skin)
			{
				ri = _layout->GetRenderItem(order, glyph->texture);
				skin = glyph->texture;
			}

			myRect.x = x_base + glyph->bearingX * scale;
			myRect.y = y_base + glyph->bearingY * scale;
			myRect.w = glyph->width * scale;
			myRect.h = glyph->height * scale;
			
			myRect.x += mAbsRect.x;
			myRect.y += mAbsRect.y;
			
			uvRect.x = glyph->u0;
			uvRect.y = glyph->v0;
			uvRect.w = glyph->u1 - glyph->u0;
			uvRect.h = glyph->v1 - glyph->v0;

			Rect _rect, _uv;
			if (Helper::Instance()->Clip(_rect, _uv, myRect, uvRect, clipRect))
			{
				ri->AddQuad(Helper::Instance()->GetQuad(_rect, _uv, color));
			}
			
			x_base += glyph->advance * scale;
		}
	}

	int TextView::_mapTextIndex(float _x, float _y)
	{
		if (mFont == NULL)
			return 0;

		int selectIndex = mGlyphs.Size();;

		const Rect & tbRect = GetAbsRect();
		float x0 = tbRect.x;
		float x1 = tbRect.x;
		float y0 = tbRect.y;
		float y1 = tbRect.y;
		float h_space = mFont->GetHeight() + mVerticalSeparation;

		y1 += h_space;

		int lineStartIndex = 0;
		int lineEndIndex = 0;
		float x_base = 0;
		float x_max = mMaxWidth;

		while (lineStartIndex < mGlyphs.Size())
		{
			const Glyph * glyph = mGlyphs[lineStartIndex];

			if (_y <= y1)
				break ;

			if (glyph->code == uchar_t(L'\n'))
			{
				x_base = 0;
				y0 += h_space;
				y1 += h_space;
				lineStartIndex += 1;
				continue ;
			}

			if (mAutoLineFeed && x_base + glyph->advance > x_max)
			{
				x_base = 0;
				y0 += h_space;
				y1 += h_space;
				continue ;
			}

			x_base += glyph->advance;
			lineStartIndex += 1;
		}

		lineEndIndex = lineStartIndex;
		while (lineEndIndex < mGlyphs.Size())
		{
			const Glyph * glyph = mGlyphs[lineEndIndex];

			if (glyph->code == uchar_t(L'\n'))
				break;

			if (mAutoLineFeed && x_base + glyph->advance > x_max)
				break ;

			lineEndIndex += 1;
		}

		selectIndex = lineEndIndex;
		for (int i = lineStartIndex; i < lineEndIndex; ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			x1 = x0 + glyph->advance;

			if (_x < x1 - 2)
			{
				selectIndex = i;
				break;
			}

			x0 = x1;
		}

		return selectIndex;
	}

	void TextView::_updateSelectorRenderItem(RenderItem * ri, const Float4 & color, int selectIndex)
	{
		if (mFont == NULL)
			return ;

		if (mAutoLineFeed && mMaxWidth < mFont->GetHeight())
			return ;

		Rect clipRect = mParent->GetClipRect();

		Rect myRect;
		myRect.x = 0;
		myRect.y = 0;
		myRect.w = 1;
		myRect.h = mFont->GetHeight();

		for (int i = 0; i < selectIndex; ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				myRect.x = 0;
				myRect.y += mFont->GetHeight() + mVerticalSeparation;
				continue;
			}

			if (mAutoLineFeed && myRect.x + glyph->advance > mMaxWidth)
			{
				myRect.x = glyph->advance;
				myRect.y += mFont->GetHeight() + mVerticalSeparation;
				continue;
			}

			myRect.x += glyph->advance;
		}

		myRect.x = mAbsRect.x + myRect.x - 1;
		myRect.y = mAbsRect.y + myRect.y;
		myRect.w = 3;

		Rect uvRect;
		uvRect.x = 0;
		uvRect.y = 0;
		uvRect.w = 1;
		uvRect.h = 1;

		Rect _rect, _uv;
		if (Helper::Instance()->Clip(_rect, _uv, myRect, uvRect, clipRect))
			ri->AddQuad(Helper::Instance()->GetQuad(_rect, _uv, color));
	}

	void TextView::_updateSelectedRenderItem(RenderItem * ri, const Float4 & color, int startIndex, int endIndex)
	{
		if (mFont == NULL)
			return ;

		if (mAutoLineFeed && mMaxWidth < mFont->GetHeight())
			return ;

		Rect clipRect = mParent->GetClipRect();
		float x_base = 0;
		float y_base = 0;

		Rect myRect, uvRect;
		uvRect.x = 0;
		uvRect.y = 0;
		uvRect.w = 0;
		uvRect.h = 0;

		for (int i = 0; i < startIndex; ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				x_base = 0;
				y_base += mFont->GetHeight() + mVerticalSeparation;
				continue;
			}

			if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
			{
				x_base = 0;
				y_base += mFont->GetHeight() + mVerticalSeparation;
			}

			x_base += glyph->advance;
		}

		for (int i = startIndex; i < endIndex; )
		{
			myRect.x = x_base;
			myRect.y = y_base;
			myRect.w = 0;
			myRect.h = mFont->GetHeight();

			while (i < endIndex)
			{
				const Glyph * glyph = mGlyphs[i];

				if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
					break;

				i += 1;

				x_base += glyph->advance;
				myRect.w += glyph->advance;

				if (glyph->code == WC('\n'))
					break;
			}

			myRect.x += mAbsRect.x;
			myRect.y += mAbsRect.y;

			Rect _rect, _uv;
			if (Helper::Instance()->Clip(_rect, _uv, myRect, uvRect, clipRect))
			{
				ri->AddQuad(Helper::Instance()->GetQuad(_rect, _uv, color));
			}

			x_base = 0;
			y_base += mFont->GetHeight() + mVerticalSeparation;
		}
	}

	void TextView::_updateGlyph()
	{
		_updateTM();

		mGlyphs.Clear();

		if (mFont == NULL)
			return ;

		if (mAutoLineFeed && mMaxWidth < mFont->GetHeight())
			return ;

		int length = mCaption.Length();
		
		for (int i = 0; i < length; ++i)
		{
			uchar_t ch = mCaption[i];

			const Glyph * glyph = mFont->GetGlyph(ch);

			if (glyph == NULL)
				glyph = mFont->GetGlyph(L'*');

			d_assert (glyph != NULL);

			mGlyphs.PushBack(glyph);
		}
	}

	void TextView::_updateRect()
	{
		mGlyphRect.x = 0;
		mGlyphRect.y = 0;
		mGlyphRect.w = 0;
		mGlyphRect.h = 0;

		if (mFont == NULL || mGlyphs.Size() == 0)
			return ;

		if (mAutoLineFeed && mMaxWidth < mFont->GetHeight())
			return ;
		
		float line_width = 0;
		mGlyphRect.w = mAutoLineFeed ? mMaxWidth : 0;
		mGlyphRect.h = mFont->GetHeight();
		
		for (int i = 0; i < mGlyphs.Size(); ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == uchar_t('\n'))
			{
				line_width = 0;
				mGlyphRect.h += mFont->GetHeight() + mVerticalSeparation;
				continue;
			}

			if (mAutoLineFeed && line_width + glyph->advance > mMaxWidth)
			{
				line_width = glyph->advance;
				mGlyphRect.h += mFont->GetHeight() + mVerticalSeparation;
				continue;
			}

			if (!mAutoLineFeed)
			{
				line_width += glyph->advance;
				mGlyphRect.w = Max(line_width, mGlyphRect.w);
			}
		}

		Rect myRect;
		myRect.x = mRect.x;
		myRect.y = mRect.y;
		myRect.w = mGlyphRect.w;
		myRect.h = mGlyphRect.h;

		SetRect(mGlyphRect);
	}

	void TextView::_getLineOffset(int & line, int & offset, int index)
	{
		float x_base = 0;
		float y_base = 0;

		line = 0;
		offset = 0;

		for (int i = 0; i < index; ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				++line;
				x_base = 0;
				offset = 0;
				continue;
			}

			if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
			{
				++line;
				x_base = glyph->advance;
				offset = 1;
				continue;
			}

			++offset;
			x_base += glyph->advance;
		}
	}

	int TextView::_getPrevLineIndex(int line, int offset, int index)
	{
		if (line == 0)
			return index;

		float x_base = 0;
		float y_base = 0;

		int t_line = 0, i = 0;
		while (t_line < line - 1 && i < mGlyphs.Size())
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				x_base = 0;
				i += 1;
				t_line += 1;
				continue;
			}

			if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
			{
				x_base = 0;
				t_line += 1;
				continue;
			}

			i += 1;
			x_base += glyph->advance;
		}

		x_base = 0;
		while (i < mGlyphs.Size() && offset > 0)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				break;
			}

			if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
			{
				break;
			}

			i += 1;
			offset -= 1;
			x_base += glyph->advance;
		}

		return i;
	}

	int TextView::_getNextLineIndex(int line, int offset, int index)
	{
		float x_base = 0;
		float y_base = 0;

		int t_line = 0, i = 0;
		while (t_line < line + 1 && i < mGlyphs.Size())
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				x_base = 0;
				i += 1;
				t_line += 1;
				continue;
			}

			if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
			{
				x_base = 0;
				t_line += 1;
				continue;
			}

			i += 1;
			x_base += glyph->advance;
		}

		while (i < mGlyphs.Size() && offset > 0)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->code == WC('\n'))
			{
				break;
			}

			if (mAutoLineFeed && x_base + glyph->advance > mMaxWidth)
			{
				break;
			}

			i += 1;
			offset -= 1;
			x_base += glyph->advance;
		}

		return i;
	}

}}