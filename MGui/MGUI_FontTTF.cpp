#include "MGUI_FontTTF.h"
#include "Internal/MGUI_FontTTFBorderGenerateor.h"
#include "Internal/MGUI_FontTTFStandardGenerator.h"

#pragma warning (push)
#pragma warning (disable : 4244)

namespace Rad { namespace MGUI {

#define FONT_TEXTURE_SIZE 256
#define FONT_INV_TEXTURE_SIZE (1.0f / 256.0f)

	FT_Library gFTLibrary;

	TTFBorderGenerator gBorderGenerator;
	TTFStandardGenerator gStandardGenerator;

	FontTTF::FontTTF(const FixedString32 & _name, void * _face, int _charSize, int border)
		: Font(_name)
		, mFace(_face)
		, mCharSize(_charSize)
		, _texture(NULL)
		, _border(border)
		, _x(0)
		, _y(0)
		, _space(1)
		, _height(0)
	{
		FT_Face ftFace = (FT_Face)mFace;

		mAscender = 0;
		mDescender = 0;
		mHeight = 0;

		if (FT_Set_Pixel_Sizes(ftFace, mCharSize, mCharSize) == 0)
		{
			mAscender = float(ftFace->size->metrics.ascender >> 6);
			mDescender = float(ftFace->size->metrics.descender >> 6);
			mHeight = float(ftFace->size->metrics.height >> 6);
		}
	}

	FontTTF::~FontTTF()
	{
		for (int i = 0; i < mTextures.Size(); ++i)
		{
			mTextures[i] = NULL;
		}

		mTextures.Clear();
		mGlyphs.Clear();
	}

	const Glyph * FontTTF::GetGlyph(uchar_t _char)
	{
		GlyphMap::Node * i = mGlyphs.Find(_char);

		if (i != mGlyphs.End())
			return &(i->value);

		Glyph glyph;

		if (_LoadGlyph(glyph, _char))
		{
			glyph.code = _char;
			i = mGlyphs.Insert(_char, glyph);

			return &(i->value);
		}

		return NULL;
	}

	bool FontTTF::_LoadGlyph(Glyph & _glyph, uchar_t _char)
	{
		if (FT_Set_Pixel_Sizes((FT_Face)mFace, mCharSize, mCharSize) == 0)
		{
			int bw = 0, bh = 0;
			const PixelLA * pxl = NULL;

			if (_border && gBorderGenerator.LoadGlyph(_char, _border, gFTLibrary, (FT_Face)mFace))
			{
				if (gBorderGenerator._bitmap.Size() > 0)
				{
					pxl = &gBorderGenerator._bitmap[0];
					bw = gBorderGenerator._width;
					bh = gBorderGenerator._height;
				}

				_glyph.width = gBorderGenerator._width;
				_glyph.height = gBorderGenerator._height;
				_glyph.bearingX = gBorderGenerator._bearingX;
				_glyph.bearingY = mAscender - gBorderGenerator._bearingY;
				_glyph.advance = gBorderGenerator._advance;
			}
			else if (gStandardGenerator.LoadGlyph(_char, gFTLibrary, (FT_Face)mFace))
			{
				if (gStandardGenerator._bitmap.Size() > 0)
				{
					pxl = &gStandardGenerator._bitmap[0];
					bw = gStandardGenerator._width;
					bh = gStandardGenerator._height;
				}

				_glyph.width = gStandardGenerator._width;
				_glyph.height = gStandardGenerator._height;
				_glyph.bearingX = gStandardGenerator._bearingX;
				_glyph.bearingY = mAscender - gStandardGenerator._bearingY;
				_glyph.advance = gStandardGenerator._advance;
			}

			_IncTexture(bw, bh);
			_glyph.texture = _texture;

			void * pData = _texture->Lock(eLockFlag::READ_WRITE);
			{
				unsigned short * dest = (unsigned short *)pData + FONT_TEXTURE_SIZE * _y + _x;

				for (int j = 0; j < bh; ++j)
				{
					for (int i = 0; i < bw; ++i)
					{
						unsigned char * pixel = (unsigned char *)&dest[i];
						pixel[0] = pxl[i].l;
						pixel[1] = pxl[i].a;
					}

					pxl += bw;
					dest += FONT_TEXTURE_SIZE;
				}
			}
			_texture->Unlock();

			_glyph.u0 = (_x) * FONT_INV_TEXTURE_SIZE;
			_glyph.v0 = (_y) * FONT_INV_TEXTURE_SIZE;
			_glyph.u1 = (_x + bw) * FONT_INV_TEXTURE_SIZE;
			_glyph.v1 = (_y + bh) * FONT_INV_TEXTURE_SIZE;

			_x += bw + _space;

			if (bh > _height)
				_height = bh;

			return true;
		}

		return false;
	}

	void FontTTF::_IncTexture(int bw, int bh)
	{
		bool needCreateTexture = false;

		if (_texture)
		{
			if (_x + bw > FONT_TEXTURE_SIZE)
			{
				_y += _height + _space;
				_x = _space;
				_height = 0;
			}

			if (_y + bh + _space > FONT_TEXTURE_SIZE)
			{
				needCreateTexture = true;
			}
		}
		else
		{
			needCreateTexture = true;
		}

		if (needCreateTexture)
		{
			static int UID = 0;

			String str;
			str.Format("%s_%d", "FontTexture", UID++);
			TexturePtr t = HWBufferManager::Instance()->NewTexture(
				str, FONT_TEXTURE_SIZE, FONT_TEXTURE_SIZE, 0,
				ePixelFormat::L8A8, eUsage::DYNAMIC_MANAGED);

			void * pData = t->Lock(eLockFlag::WRITE);
			{
				unsigned short * pixel = (unsigned short *)pData;

				for (int j = 0; j < FONT_TEXTURE_SIZE; ++j)
				{
					for (int i = 0; i < FONT_TEXTURE_SIZE; ++i)
					{
						*pixel++ = 0x00FF;
					}
				}
			}
			t->Unlock();

			_texture = t.c_ptr();
			mTextures.PushBack(_texture);

			_y = _space;
			_x = _space;
		}
	}


	//
	FontFactoryTTF::FontFactoryTTF()
		: FontFactory("FontTTF")
	{
		FT_Init_FreeType(&gFTLibrary);
	}

	FontFactoryTTF::~FontFactoryTTF()
	{
		for (int i = 0; i < mFaceMap.Size(); ++i)
		{
			FT_Face face = (FT_Face)mFaceMap[i].value;

			FT_Done_Face(face);
		}
		mFaceMap.Clear();

		for (int i = 0; i < mFaceData.Size(); ++i)
		{
			mFaceData[i] = NULL;
		}

		mFaceData.Clear();

		FT_Done_FreeType(gFTLibrary);
	}

	Font * FontFactoryTTF::Load(rml_node * doc)
	{
		rml_node * nodeName = doc->first_node("Name");
		rml_node * nodeFace = doc->first_node("Face");
		rml_node * nodeSize = doc->first_node("Size");
		rml_node * nodeBorder = doc->first_node("Border");

		d_assert (nodeName && nodeFace && nodeSize);

		FixedString32 name = nodeName->value();
		String face = nodeFace->value();
		String size = nodeSize->value();
		
		int border = 0;
		if (nodeBorder != NULL)
		{
			border = atoi(nodeBorder->value());
			border = Max(0, border);
		}

		void * ftFace = _loadFace(face);

		if (ftFace)
		{
			return new FontTTF(name, ftFace, size.ToInt(), border);
		}
		else
		{
			d_log("?: Font face '%s' load failed", face.c_str());
		}

		return NULL;
	}


	void * FontFactoryTTF::_loadFace(const String & _face)
	{
		int index = mFaceMap.Find(_face);
		if (index == -1)
		{
			DataStreamPtr stream = ResourceManager::Instance()->OpenResource(_face.c_str());

			if (stream == NULL)
			{
				d_log("?: Font face '%s' can't open.", _face.c_str());

				return NULL;
			}

			DataStreamPtr faceData = new MemoryStream(stream->Size());

			stream->Read(faceData->GetData(), stream->Size());

			FT_Face ftFace = NULL;
			if (FT_New_Memory_Face(gFTLibrary, (const FT_Byte*)faceData->GetData(), stream->Size(), 0, &ftFace) != 0)
				return NULL;

			mFaceData.PushBack(faceData);
			index = mFaceMap.Insert(_face, ftFace);
		}

		return (FT_Face)mFaceMap[index].value;
	}
	
}}

#pragma warning (pop)
