#include "MGUI_FontImage.h"

namespace Rad { namespace MGUI {

	FontImage::FontImage(const FixedString32 & _name, TexturePtr texture, float height)
		: Font(_name)
		, mTexture(texture)
	{
		mHeight = height;
	}

	FontImage::~FontImage()
	{
	}

	const Glyph * FontImage::GetGlyph(uchar_t _char)
	{
		int index = mGlyphs.Find(_char);
		if (index != -1)
		{
			return &mGlyphs[index].value;
		}

		return NULL;
	}

	void FontImage::AddGlyph(uchar_t _char, const Glyph & _glyph)
	{
		mGlyphs.Insert(_char, _glyph);
	}








	FontFactoryImage::FontFactoryImage()
		: FontFactory("FontImage")
	{
	}

	FontFactoryImage::~FontFactoryImage()
	{
	}

	bool _parseGlyph(uchar_t & _char, Glyph & _glyph, const char * str, float invTexWidth, float invTexHeight)
	{
		// char
		String buff;
		int index = 0;

		while (*str && *str != ' ' && *str != '\t')
		{
			buff[index++] = *str++;
		}

		buff[index] = 0;

		if (index > 3)
			return false;

		_char = buff.c_wstr()[0];

		// glyph
		while (*str && (*str == ' ' || *str == '\t'))
			++str;

		Rect uvRect;
		str = str_getfloat(uvRect.x, str);
		str = str_getfloat(uvRect.y, str);
		str = str_getfloat(uvRect.w, str);
		str = str_getfloat(uvRect.h, str);

		_glyph.code = _char;

		_glyph.u0 = uvRect.x * invTexWidth;
		_glyph.v0 = uvRect.y * invTexHeight;
		_glyph.u1 = (uvRect.x + uvRect.w) * invTexWidth;
		_glyph.v1 = (uvRect.y + uvRect.h) * invTexHeight;

		_glyph.width = uvRect.w;
		_glyph.height = uvRect.h;

		_glyph.advance = uvRect.w;

		return true;
	}

	Font * FontFactoryImage::Load(rml_node * root)
	{
		rml_node * nodeName = root->first_node("Name");
		rml_node * nodeImage = root->first_node("Image");
		rml_node * nodeHeight = root->first_node("Height");

		d_assert (nodeName && nodeImage && nodeHeight);

		TexturePtr texture = HWBufferManager::Instance()->LoadTexture(nodeImage->value(), -1);

		if (texture == NULL)
		{
			d_log("?: Load FontImage '%s' failed", nodeName->value());
			return NULL;
		}

		FontImage * font = new FontImage(nodeName->value(), texture, (float)atof(nodeHeight->value()));

		rml_node * nodeGlyph = root->first_node("Glyph");

		uchar_t ch;
		Glyph glyph;
		float invW = texture->_getInvWidth();
		float invH = texture->_getInvHeight();

		glyph.bearingX = 0;
		glyph.bearingY = 0;
		glyph.texture = texture.c_ptr();

		while (nodeGlyph)
		{
			if (_parseGlyph(ch, glyph, nodeGlyph->value(), invW, invH))
			{
				font->AddGlyph(ch, glyph);
			}
			else
			{
				d_log("?: Load glyph '%s' failed.", nodeGlyph->value());
			}

			nodeGlyph = nodeGlyph->next_sibling("Glyph");
		}

		return font;
	}

}}