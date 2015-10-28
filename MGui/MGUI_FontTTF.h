/*
*	Font TTF
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Font.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY FontTTF : public Font
	{
		typedef t_hash<uchar_t> hash_char;
		typedef HashMap<uchar_t, Glyph, hash_char> GlyphMap;

		struct LA
		{
			float l, a;
		};

	public:
		FontTTF(const FixedString32 & _name, void * _face, int _charSize, int border);
		virtual ~FontTTF();

		virtual const Glyph * 
			GetGlyph(uchar_t _char);

	protected:
		bool 
			_LoadGlyph(Glyph & _glyph, uchar_t _char);
		void 
			_IncTexture(int w, int h);

	protected:
		void * mFace;
		int mCharSize;

		Array<TexturePtr> mTextures;
		GlyphMap mGlyphs;

		float mAscender;
		float mDescender;

		Texture * _texture;
		int _height;
		int _x, _y;
		int _space;
		int _border;

		Array<LA> mTempColor;
	};


	class FontFactoryTTF : public FontFactory
	{
		typedef Map<String, void *> FaceMap;

		friend class FontManager;

	protected:
		FontFactoryTTF();
		virtual ~FontFactoryTTF();

		virtual Font * 
			Load(rml_node * root);

	protected:
		void * 
			_loadFace(const String & _face);

	protected:
		FaceMap mFaceMap;
		Array<DataStreamPtr> mFaceData;
	};

}}
