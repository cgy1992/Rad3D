#include "MColor.h"

namespace Rad {

	Rgba32::Rgba32(byte _r, byte _g, byte _b, byte _a)
		: r(_r), g(_g), b(_b), a(_a)
	{
	}

	Rgba32::Rgba32(const Rgba32 & c)
		: _value(c._value)
	{
	}

	Rgba32::Rgba32()
		: _value(0xFF000000)
	{
	}

	Rgba32& Rgba32::operator =(const Rgba32 & c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;

		return *this;
	}

	bool Rgba32::operator ==(const Rgba32 & c) const
	{
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	bool Rgba32::operator !=(const Rgba32 & c) const
	{
		return r != c.r || g != c.g || b != c.b || a != c.a;
	}

	void Rgba32::FromFloat4(const Float4 & color)
	{
		r = (byte)(color.r * 255);
		g = (byte)(color.g * 255);
		b = (byte)(color.b * 255);
		a = (byte)(color.a * 255);
	}

	Float4 Rgba32::ToFloat4() const
	{
		Float4 v;

		v.r = r * INV_255;
		v.g = g * INV_255;
		v.b = b * INV_255;
		v.a = a * INV_255;

		return v;
	}

}