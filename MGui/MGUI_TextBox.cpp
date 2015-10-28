#include "MGUI_TextBox.h"
#include "MGUI_Layout.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(TextBox, Widget);

	TextBox::TextBox(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
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

		mPasswordEnable = false;
	}

	TextBox::~TextBox()
	{
	}

	void TextBox::SetCaption(const UString & caption)
	{
		mCaption = caption;

		_updateGlyph();
	}

	const UString & TextBox::GetCaption() const
	{
		return mCaption;
	}

	void TextBox::SetFont(Font * _font)
	{
		if (mFont != _font)
		{
			mFont = _font;

			_updateGlyph();
		}
	}

	Font * TextBox::GetFont() const
	{
		return mFont;
	}

	void TextBox::SetPasswordEnable(bool _enable)
	{
		if (mPasswordEnable != _enable)
		{
			mPasswordEnable = _enable;

			_updateGlyph();
		}
	}

	bool TextBox::GetPasswordEnable() const
	{
		return mPasswordEnable;
	}

	int TextBox::GetGlyphCount()
	{
		return mGlyphs.Size();
	}

	const Glyph * TextBox::GetGlyph(int index)
	{
		return mGlyphs[index];
	}

	void TextBox::Update(float elapsedTime)
	{
		if (mVisible)
		{
			Rect myRect;
			myRect.x = mRect.x;
			myRect.y = mRect.y;
			myRect.w = mGlyphRect.w;
			myRect.h = mGlyphRect.h;

			SetRect(myRect);
		}

		Widget::Update(elapsedTime);
	}

	const Rect & TextBox::GetGlyphRect()
	{
		return mGlyphRect;
	}

	void TextBox::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible || mFont == NULL)
			return ;

		Rect clipRect = mParent->GetClipRect();
		Float4 color = mAbsColor;
		int order = mAbsOrder;
		float scale = mAbsScale;
		float x_base = mAbsRect.x;
		float y_base = mAbsRect.y;

		if (mLookFeel != NULL)
			color *= mLookFeel->GetTextColor(mState);

		Texture * skin = NULL;
		RenderItem * item = NULL;
		Rect myRect, uvRect;

		for (int i = 0; i < mGlyphs.Size(); ++i)
		{
			const Glyph * glyph = mGlyphs[i];

			if (glyph->texture != skin)
			{
				item = _layout->GetRenderItem(order, glyph->texture);
				skin = glyph->texture;
			}

			myRect.x = x_base + glyph->bearingX * scale;
			myRect.y = y_base + glyph->bearingY * scale;
			myRect.w = glyph->width * scale;
			myRect.h = glyph->height * scale;
			
			uvRect.x = glyph->u0;
			uvRect.y = glyph->v0;
			uvRect.w = glyph->u1 - glyph->u0;
			uvRect.h = glyph->v1 - glyph->v0;

			Rect _rect, _uv;

			if (Helper::Instance()->Clip(_rect, _uv, myRect, uvRect, clipRect))
			{
				item->AddQuad(Helper::Instance()->GetQuad(_rect, _uv, color));
			}
			
			x_base += glyph->advance * scale;
		}
	}

	void TextBox::_updateGlyph()
	{
		mGlyphs.Clear();

		if (mFont == NULL)
			return ;
		
		mGlyphRect.x = 0;
		mGlyphRect.y = 0;
		mGlyphRect.w = 0;
		mGlyphRect.h = mFont->GetHeight();

		int length = mCaption.Length();
		float width = 0;

		const Glyph * pd_glyph = mFont->GetGlyph(WCHAR_('*'));

		for (int i = 0; i < length; ++i)
		{
			uchar_t ch = mCaption[i];

			const Glyph * glyph = mPasswordEnable ? pd_glyph : mFont->GetGlyph(ch);
			
			if (glyph == NULL)
				glyph = mFont->GetGlyph(Helper::Instance()->GetUnknownChar());

			if (glyph == NULL)
				continue;

			mGlyphs.PushBack(glyph);
			width += glyph->advance;
		}

		mGlyphRect.w = width;
	}

	int TextBox::_mapTextIndex(float _x, float _y)
	{
		if (mFont == NULL)
			return 0;

		int selectIndex = mGlyphs.Size();;

		const Rect & tbRect = GetAbsRect();
		float x0 = tbRect.x;
		float x1 = tbRect.x;
		float y0 = tbRect.y;
		float y1 = tbRect.y;

		for (int i = 0; i < mGlyphs.Size(); ++i)
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

	void TextBox::_updateSelectorRenderItem(RenderItem * ri, const Float4 & color, int selectIndex)
	{
		if (mFont == NULL)
			return ;

		Rect clipRect = mParent->GetClipRect();

		Rect myRect;
		myRect.x = 0;
		myRect.y = 0;
		myRect.w = 1;
		myRect.h = mFont->GetHeight();

		for (int i = 0; i < selectIndex; ++i)
		{
			myRect.x += mGlyphs[i]->advance;
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

	void TextBox::_updateSelectedRenderItem(RenderItem * ri, const Float4 & color, int startIndex, int endIndex)
	{
		Rect myRect = mAbsRect;

		for (int i = 0; i < startIndex; ++i)
			myRect.x += mGlyphs[i]->advance;

		myRect.w = 0;
		for (int i = startIndex; i < endIndex; ++i)
			myRect.w += mGlyphs[i]->advance;

		Rect clipRect = mParent->GetClipRect();
		Rect uvRect(0, 0, 1, 1);

		Rect _rect, _uv;
		if (Helper::Instance()->Clip(_rect, _uv, myRect, uvRect, clipRect))
			ri->AddQuad(Helper::Instance()->GetQuad(_rect, _uv, color));
	}

}}