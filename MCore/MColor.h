/*
*	Color
*		32 bit color
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat4.h"

namespace Rad {

	class M_ENTRY Color
	{
		DECLARE_ALLOC();

	public:
		static const Color White;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Black;

	public:
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		Color(const Color & c);
		Color();
		~Color();

		Color &
			operator =(const Color & c);

		bool
			operator ==(const Color & c) const;
		bool 
			operator !=(const Color & c) const;

		void 
			FromFloat4(const Float4 & color);
		Float4 
			ToFloat4() const;

	public:
		union {
			struct {
				unsigned char r, g, b, a;
			};

			int value;
		};
	};

}