/*
*	MGUI Types
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Entry.h"

#pragma warning(push)
#pragma warning(disable : 4996)

namespace Rad { namespace MGUI {

	#define MAX_TOUCH_POINTS 6

	struct Rect
	{
		float x, y, w, h;

		Rect() : x(0), y(0), w(0), h(0) {}
		Rect(float _x, float _y, float _w, float _h)
			: x(_x), y(_y), w(_w), h(_h)
		{
		}

		bool operator ==(const Rect & _rk) const
		{
			return x == _rk.x && y == _rk.y && w == _rk.w && h == _rk.h;
		}

		bool operator !=(const Rect & _rk) const
		{
			return x != _rk.x || y != _rk.y || w != _rk.w || h != _rk.h;
		}

		String ToString() const
		{
			String str;

			str.Format("%d %d %d %d", (int)x, (int)y, (int)w, (int)h);

			return str;
		}

		void FromString(const String & _string)
		{
			sscanf(_string.c_str(), "%f %f %f %f", &x, &y, &w, &h);
		}

		float cx() const { return (x + w) * 0.5f; }
		float cy() const { return (y + h) * 0.5f; }
	};

	struct ClientRect
	{
		float dx0, dy0;
		float dx1, dy1;

		ClientRect() : dx0(0), dy0(0), dx1(0), dy1(0) {}
		ClientRect(float _x0, float _y0, float _x1, float _y1)
		{
			dx0 = _x0; dy0 = _y0;
			dx1 = _x1; dy1 = _y1;
		}

		bool operator ==(const ClientRect & _rk) const
		{
			return dx0 == _rk.dx0 && dy0 == _rk.dy0 && dx1 == _rk.dx1 && dy1 == _rk.dy1;
		}

		bool operator !=(const ClientRect & _rk) const
		{
			return dx0 != _rk.dx0 || dy0 != _rk.dy0 || dx1 != _rk.dx1 || dy1 != _rk.dy1;
		}

		String ToString() const
		{
			int _x0 = (int)dx0, _y0 = (int)dy0, _x1 = (int)dx1, _y1 = (int)dy1;

			String str;

			str.Format("%d %d %d %d", _x0, _y0, _x1, _y1);

			return str;
		}

		void FromString(const String & _string)
		{
			sscanf(_string.c_str(), "%f %f %f %f", &dx0, &dy0, &dx1, &dy1);
		}
	};

	struct eAlign
	{
		enum enum_t {
			NONE,

			H_CENTER = 1 << 0,
			V_CENTER = 1 << 1,
			CENTER = H_CENTER | V_CENTER,

			LEFT =  1 << 2,
			RIGHT = 1 << 3,
			H_STRETCH = 1 << 4,

			TOP = 1 << 5,
			BOTTOM = 1 << 6,
			V_STRETCH = 1 << 7,

			STRETCH = H_STRETCH | V_STRETCH,
		};

		int _value;

		eAlign() : _value(NONE) {}
		eAlign(int val) : _value(val) {}

		eAlign & operator =(int rk)
		{
			_value = rk;
			return *this;
		}

		bool operator ==(int rk) const
		{
			return _value == rk;
		}

		bool operator !=(int rk) const
		{
			return _value != rk;
		}

		bool operator ==(const eAlign & rk) const
		{
			return _value == rk._value;
		}

		bool operator !=(const eAlign & rk) const
		{
			return _value != rk._value;
		}
	};

	struct Vertex
	{
		float x, y;
		float r, g, b, a;
		float u, v;

		Vertex()
		{
			x = y = 0;
			r = g = b = a = 0;
			u = v = 0;
		}
	};

	struct MGUI_ENTRY eWidgetState
	{
		enum enum_t {
			NORMAL,
			DISABLE,
			FOCUS,
			PRESS,
			SELECT,
			SELECT_DISABLE,
			SELECT_FOCUS,
			SELECT_PRESS,

			MAX,
		};

		static const FixedString32 StateString[];

		static int GetState(const char * _state);
		static FixedString32 GetStateString(int _state); 
	};

	struct ePickFlag 
	{
		enum enum_t {
			NONE,
			SELF,
			CHILD,
			ALL,
		};
	};
	
	struct eOrder
	{
		enum enum_t {
			DEFAULT,
			TEXT_BACK,
			TEXT,
			TEXT_CURSOR,

			OVERLAP = 0xFFFF, 
			
			TOP = 0x70000000,
		};
	};

}}

#pragma warning(pop)

