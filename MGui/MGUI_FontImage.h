/*
*	Font Image
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Font.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY FontImage : public Font
	{
	public:
		FontImage(const FixedString32 & _name, TexturePtr _texture, float height);
		virtual ~FontImage();

		virtual const Glyph * 
			GetGlyph(uchar_t _char);

		void 
			AddGlyph(uchar_t _char, const Glyph & _glyph);

	protected:
		Map<uchar_t, Glyph> mGlyphs;
		TexturePtr mTexture;
	};


	class MGUI_ENTRY FontFactoryImage : public FontFactory
	{
	public:
		FontFactoryImage();
		virtual ~FontFactoryImage();

		virtual Font * 
			Load(rml_node * root);
	};

}}
