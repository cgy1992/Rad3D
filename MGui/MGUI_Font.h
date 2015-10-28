/*
*	Font
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"

namespace Rad { namespace MGUI {

	struct PixelLA {
		
		unsigned char l, a;

		PixelLA() {}
		PixelLA(unsigned char _l, unsigned char _a) : l(_l), a(_a) {}
	};

	struct MGUI_ENTRY Glyph {

		uchar_t code;

		float u0, v0;
		float u1, v1;

		float width;	// pixels of texture
		float height;	// 

		float bearingX;
		float bearingY;

		float advance;

		Texture * texture;

		Glyph()
		{
			code = 0;

			u0 = v0 = 0;
			u1 = v1 = 0;

			width = height = 0;
			bearingX = bearingY = 0;

			advance = 0;

			texture = NULL;
		}
	};

	class MGUI_ENTRY Font
	{
		DECLARE_ALLOC();

	public:
		Font(const FixedString32 & name);
		virtual ~Font();

		const FixedString32 & 
			GetName() { return mName; }

		float 
			GetHeight() { return mHeight; };

		virtual const Glyph * 
			GetGlyph(uchar_t _char) = 0;

	protected:
		FixedString32 mName;
		float mHeight;
	};


	class MGUI_ENTRY FontFactory
	{
		DECLARE_ALLOC();

	public:
		FontFactory(const String & type) : mType(type) {}
		virtual ~FontFactory() {}

		const String & 
			GetType() { return mType; }

		virtual Font * 
			Load(rml_node * root) = 0;

	protected:
		String mType;
	};

}}
