/*
*	Rgba32
*		32 bit color
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat4.h"

namespace Rad {

	struct M_ENTRY Rgba32
	{
		union {
			struct {
				byte r;
				byte g;
				byte b;
				byte a;
			};

			int _value;
		};

		Rgba32(byte r, byte g, byte b, byte a = 255);
		Rgba32(const Rgba32 & c);
		Rgba32();

		Rgba32 &
			operator =(const Rgba32 & c);

		bool
			operator ==(const Rgba32 & c) const;
		bool 
			operator !=(const Rgba32 & c) const;

		void 
			FromFloat4(const Float4 & color);
		Float4 
			ToFloat4() const;
	};

	struct Rgb24
	{
		byte r;
		byte g;
		byte b;

		Rgb24(byte _r, byte _g, byte _b) { r = _r, g = _g, b = _b; }
		Rgb24() { r = 0, g = 0, b = 0; }
	};

	struct Rgb16
	{
		union {
			struct {
				word r : 5;
				word g : 6;
				word b : 5;
			};

			word _value;
		};

		Rgb16(byte _r, byte _g, byte _b) { r = _r & 0x1F, g = _g & 0x2F, b = _b & 0x1F; }
		Rgb16() { _value = 0; }
	};

	struct Rgba16
	{
		union {
			struct {
				word r : 4;
				word g : 4;
				word b : 4;
				word a : 4;
			};

			word _value;
		};

		Rgba16(byte _r, byte _g, byte _b, byte _a = 0x0F) { r = _r & 0x0F, g = _g & 0x0F, b = _b & 0x0F; a = _a & 0x0F; }
		Rgba16() { _value = 0xF000; }
	};

}