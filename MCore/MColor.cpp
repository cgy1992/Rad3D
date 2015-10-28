#include "MColor.h"

namespace Rad {

    const Color Color::White      = Color(255, 255, 255, 255);
    const Color Color::Red        = Color(255,   0,   0, 255);
    const Color Color::Green      = Color(0,   255,   0, 255);
    const Color Color::Blue       = Color(0,     0, 255, 255);
    const Color Color::Black      = Color(0,     0,   0, 255);


	Color::Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
		: r(_r), g(_g), b(_b), a(_a)
	{
	}

	Color::Color(const Color & c)
		: r(c.r), g(c.g), b(c.b), a(c.a) 
	{
	}

	Color::Color()
		: r(0), g(0), b(0), a(255)
	{
	}

	Color::~Color()
	{
	}

	Color& Color::operator =(const Color & c)
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;

		return *this;
	}

	bool Color::operator ==(const Color & c) const
	{
		return r == c.r && g == c.g && b == c.b && a == c.a;
	}

	bool Color::operator !=(const Color & c) const
	{
		return r != c.r || g != c.g || b != c.b || a != c.a;
	}

	void Color::FromFloat4(const Float4 & color)
	{
		r = (unsigned char)(color.r * 255);
		g = (unsigned char)(color.g * 255);
		b = (unsigned char)(color.b * 255);
		a = (unsigned char)(color.a * 255);
	}

	Float4 Color::ToFloat4() const
	{
		static const float inv_255 = 1.0f / 255.0f;

		Float4 v;

		v.r = r * inv_255;
		v.g = g * inv_255;
		v.b = b * inv_255;
		v.a = a * inv_255;

		return v;
	}

}