/*
*	FontTTF Standard Generator
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Font.h"

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftstroke.h>

#pragma warning (push)
#pragma warning (disable : 4244)

namespace Rad { namespace MGUI {

	class TTFStandardGenerator
	{
	public:
		int _width, _height;
		Array<PixelLA> _bitmap;
		int _advance;
		int _bearingX;
		int _bearingY;

		bool LoadGlyph(uchar_t ch, FT_Library library, FT_Face face)
		{
			if (FT_Load_Char(face, ch, FT_LOAD_RENDER | FT_LOAD_TARGET_NORMAL) != 0)
				return false;

			int bw = face->glyph->bitmap.width;
			int bh = face->glyph->bitmap.rows;

			_bitmap.Resize(bw * bh);
			_width = bw;
			_height = bh;

			_width = face->glyph->metrics.width >> 6;
			_height = face->glyph->metrics.height >> 6;
			_bearingX = face->glyph->metrics.horiBearingX >> 6;
			_bearingY = face->glyph->metrics.horiBearingY >> 6;
			_advance = face->glyph->advance.x >> 6;

			if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_GRAY)
			{
				unsigned char * src = (unsigned char*)face->glyph->bitmap.buffer;

				for (int j = 0; j < bh; ++j)
				{
					for (int i = 0; i < bw; ++i)
					{
						_bitmap[j * bw + i].l = 0xFF;
						_bitmap[j * bw + i].a = src[i];
					}

					src += face->glyph->bitmap.pitch;
				}

				return true;
			}
			else if (face->glyph->bitmap.pixel_mode == FT_PIXEL_MODE_MONO)
			{
				FT_Bitmap ftBitmap;
				FT_Bitmap_New(&ftBitmap);

				// Convert the monochrome bitmap to 8-bit before rendering it.
				if (FT_Bitmap_Convert(library, &face->glyph->bitmap, &ftBitmap, 1) == 0)
				{
					// Go through the bitmap and convert all of the nonzero values to 0xFF (white).
					for (unsigned char * p = ftBitmap.buffer, * endP = p + ftBitmap.width * ftBitmap.rows; p != endP; ++p)
						*p ^= -*p ^ *p;
				}

				unsigned char * src = (unsigned char*)ftBitmap.buffer;

				for (int j = 0; j < bh; ++j)
				{
					for (int i = 0; i < bw; ++i)
					{
						_bitmap[j * bw + i].l = 0xFF;
						_bitmap[j * bw + i].a = src[i];
					}

					src += ftBitmap.pitch;
				}

				FT_Bitmap_Done(library, &ftBitmap);

				return true;
			}

			return false;
		}
	};

}}

#pragma warning (pop)