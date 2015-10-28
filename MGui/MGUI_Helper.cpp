#include "MGUI_Helper.h"

#pragma warning(push)
#pragma warning(disable : 4244)

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(Helper);

	Helper::Helper()
	{
		mUnknownChar = WC('*');
	}

	Helper::~Helper()
	{
	}

	Rect Helper::MapUVRect(const Rect & _rect, Texture * _skin)
	{
		Rect uv;

		if (_skin != NULL)
		{
			float invW = _skin->_getInvWidth();
			float invH = _skin->_getInvHeight();

			uv.x = (float)(_rect.x) * invW;
			uv.y = (float)(_rect.y) * invH;
			uv.w = (float)(_rect.w) * invW;
			uv.h = (float)(_rect.h) * invH;
		}

		return uv;
	}

	bool Helper::Clip(Rect & _orect, Rect & _ouv, const Rect & _rect, const Rect & _uv, const Rect & _clipRect)
	{
		if (_rect.w < 1 || _rect.h < 1 || _clipRect.w < 1 || _clipRect.h < 1)
			return false;

		if (_rect.x >= _clipRect.x + _clipRect.w || _rect.x + _rect.w <= _clipRect.x ||
			_rect.y >= _clipRect.y + _clipRect.h || _rect.y + _rect.h <= _clipRect.y)
			return false;

		if (_rect.x >= _clipRect.x && _rect.x + _rect.w <= _clipRect.x + _clipRect.w &&
			_rect.y >= _clipRect.y && _rect.y + _rect.h <= _clipRect.y + _clipRect.h)
		{
			_orect = _rect;
			_ouv = _uv;
		}
		else
		{
			float x0 = _rect.x;
			float y0 = _rect.y;
			float x1 = x0 + _rect.w;
			float y1 = y0 + _rect.h;

			float clip_x0 = _clipRect.x;
			float clip_y0 = _clipRect.y;
			float clip_x1 = _clipRect.x + _clipRect.w;
			float clip_y1 = _clipRect.y + _clipRect.h;

			float uStep = _uv.w / _rect.w;
			float vStep = _uv.h / _rect.h;
			float u = _uv.x, v = _uv.y;

			x0 = Max(clip_x0, x0);
			y0 = Max(clip_y0, y0);

			x1 = Min(clip_x1, x1);
			y1 = Min(clip_y1, y1);

			d_assert (x1 >= x0 && y1 >= y0);

			_orect.x = x0; _orect.y = y0;
			_orect.w = x1 - x0; _orect.h = y1 - y0;

			_ouv.x = u + (_orect.x - _rect.x) * uStep;
			_ouv.y = v + (_orect.y - _rect.y) * vStep;
			_ouv.w = _orect.w * uStep;
			_ouv.h = _orect.h * vStep;
		}

		return true;
	}

	const Vertex * Helper::GetQuad(const Rect & _r, const Rect & _uv, const Float4 & _c)
	{
		static Vertex q[4];

		float x0 = _r.x;
		float y0 = _r.y;
		float x1 = _r.x + _r.w;
		float y1 = _r.y + _r.h;

		float u0 = _uv.x;
		float v0 = _uv.y;
		float u1 = _uv.x + _uv.w;
		float v1 = _uv.y + _uv.h;

		float cr = _c.r;
		float cg = _c.g;
		float cb = _c.b;
		float ca = _c.a;

		q[0].x = x0; q[0].y = y0; q[0].u = u0; q[0].v = v0;
		q[0].r = cr; q[0].g = cg; q[0].b = cb; q[0].a = ca;
		
		q[1].x = x1; q[1].y = y0; q[1].u = u1; q[1].v = v0;
		q[1].r = cr; q[1].g = cg; q[1].b = cb; q[1].a = ca;

		q[2].x = x0; q[2].y = y1; q[2].u = u0; q[2].v = v1;
		q[2].r = cr; q[2].g = cg; q[2].b = cb; q[2].a = ca;

		q[3].x = x1; q[3].y = y1; q[3].u = u1; q[3].v = v1;
		q[3].r = cr; q[3].g = cg; q[3].b = cb; q[3].a = ca;

		for (int i = 0; i < 4; ++i)
		{
			q[i].x = Math::Floor(q[i].x);
			q[i].y = Math::Floor(q[i].y);
		}

		return q;
	}

	void Helper::AddRenderItem(RenderItem * ri, const Rect & myRect, const Rect & clRect,
		const Rect & uvRect, const Rect & uvClientRect, const Float4 & color, const Rect & clipRect)
	{
		Rect rect, _rect;
		Rect uv, _uv;

		float x0 = myRect.x;
		float y0 = myRect.y;
		float x1 = myRect.x + myRect.w;
		float y1 = myRect.y + myRect.h;

		float cl_x0 = clRect.x;
		float cl_y0 = clRect.y;
		float cl_x1 = clRect.x + clRect.w;
		float cl_y1 = clRect.y + clRect.h;

		float u0 = uvRect.x;
		float v0 = uvRect.y;
		float u1 = uvRect.x + uvRect.w;
		float v1 = uvRect.y + uvRect.h;

		float cl_u0 = uvClientRect.x;
		float cl_v0 = uvClientRect.y;
		float cl_u1 = uvClientRect.x + uvClientRect.w;
		float cl_v1 = uvClientRect.y + uvClientRect.h;

		if (cl_x1 < cl_x0 || cl_y1 < cl_y0)
			return ;

		// top
		if (cl_y0 > y0)
		{
			// left
			if (cl_x0 > x0)
			{
				float _x0 =    x0, _y0 =    y0;
				float _x1 = cl_x0, _y1 = cl_y0;

				float _u0 =    u0, _v0 =    v0;
				float _u1 = cl_u0, _v1 = cl_v0;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}

			// center
			if (1)
			{
				float _x0 = cl_x0, _y0 =    y0;
				float _x1 = cl_x1, _y1 = cl_y0;

				float _u0 = cl_u0, _v0 =    v0;
				float _u1 = cl_u1, _v1 = cl_v0;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}

			// right
			if (cl_x1 < x1)
			{
				float _x0 = cl_x1, _y0 =    y0;
				float _x1 =    x1, _y1 = cl_y0;

				float _u0 = cl_u1, _v0 =    v0;
				float _u1 =    u1, _v1 = cl_v0;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}
		}

		// center
		if (1)
		{
			// left
			if (cl_x0 > x0)
			{
				float _x0 = x0,    _y0 = cl_y0;
				float _x1 = cl_x0, _y1 = cl_y1;

				float _u0 = u0,	   _v0 = cl_v0;
				float _u1 = cl_u0, _v1 = cl_v1;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}

			// center
			if (1)
			{
				float _x0 = cl_x0, _y0 = cl_y0;
				float _x1 = cl_x1, _y1 = cl_y1;

				float _u0 = cl_u0, _v0 = cl_v0;
				float _u1 = cl_u1, _v1 = cl_v1;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}

			// right
			if (cl_x1 < x1)
			{
				float _x0 = cl_x1, _y0 = cl_y0;
				float _x1 =    x1, _y1 = cl_y1;

				float _u0 = cl_u1, _v0 = cl_v0;
				float _u1 =    u1, _v1 = cl_v1;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}
		}

		// bottom
		if (cl_y1 < y1)
		{
			// left
			if (cl_x0 > x0)
			{
				float _x0 =    x0, _y0 = cl_y1;
				float _x1 = cl_x0, _y1 =    y1;

				float _u0 =    u0, _v0 = cl_v1;
				float _u1 = cl_u0, _v1 =    v1;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}

			// center
			if (1)
			{
				float _x0 = cl_x0, _y0 = cl_y1;
				float _x1 = cl_x1, _y1 =    y1;

				float _u0 = cl_u0, _v0 = cl_v1;
				float _u1 = cl_u1, _v1 =    v1;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}

			// right
			if (cl_x1 < x1)
			{
				float _x0 = cl_x1, _y0 = cl_y1;
				float _x1 =    x1, _y1 =    y1;

				float _u0 = cl_u1, _v0 = cl_v1;
				float _u1 =    u1, _v1 =    v1;

				rect.x = _x0, rect.y = _y0;
				rect.w = _x1 - _x0, rect.h = _y1 - _y0;
				uv.x = _u0, uv.y = _v0;
				uv.w = _u1 - _u0, uv.h = _v1 - _v0;

				if (Clip(_rect, _uv, rect, uv, clipRect))
					ri->AddQuad(GetQuad(_rect, _uv, color));
			}
		}
	}

	Rect Helper::GetAlignRect(const Rect & rect, const Rect & parent, eAlign align)
	{
		if (align._value == eAlign::NONE)
			return rect;

		Rect r = rect;

		if (align._value & eAlign::LEFT)
		{
			r.x = parent.x;
		}
		else if (align._value & eAlign::RIGHT)
		{
			r.x = parent.x + parent.w - rect.w;
		}

		if (align._value & eAlign::TOP)
		{
			r.y = parent.y;
		}
		else if (align._value & eAlign::BOTTOM)
		{
			r.y = parent.y + parent.h - rect.h;
		}

		if (align._value & eAlign::H_CENTER)
		{
			r.x = parent.x + (parent.w - rect.w) / 2;
		}

		if (align._value & eAlign::V_CENTER)
		{
			r.y = parent.y + (parent.h - rect.h) / 2;
		}

		if (align._value & eAlign::H_STRETCH)
		{
			r.x = parent.x;
			r.w = parent.w;
		}

		if (align._value & eAlign::V_STRETCH)
		{
			r.y = parent.y;
			r.h = parent.h;
		}

		return r;
	}

	Rect Helper::GetRelativeRect(Widget * widget, const Rect & absRect)
	{
		Rect r;

		const Rect & rect = widget->GetAbsClient();

		r.x = absRect.x - rect.x;
		r.y = absRect.y - rect.y;
		r.w = absRect.w;
		r.h = absRect.h;
		
		return r;
	}

	Rect Helper::GetAbsolutelyRect(Widget * widget, const Rect & relRect)
	{
		Rect r;

		const Rect & rect = widget->GetAbsClient();

		r.x = relRect.x + rect.x;
		r.y = relRect.y + rect.y;
		r.w = relRect.w;
		r.h = relRect.h;

		return r;
	}

	void Helper::GetAlignPoint(float & x, float & y, const Rect & rect, eAlign align)
	{
		x = rect.x;
		y = rect.y;

		if (align._value & eAlign::LEFT)
		{
			x = rect.x;
		}
		else if (align._value & eAlign::RIGHT)
		{
			x = rect.x + rect.w;
		}

		if (align._value & eAlign::TOP)
		{
			y = rect.y;
		}
		else if (align._value & eAlign::BOTTOM)
		{
			y = rect.y + rect.h;
		}

		if (align._value & eAlign::H_CENTER)
		{
			x = rect.x + rect.w / 2;
		}

		if (align._value & eAlign::V_CENTER)
		{
			y = rect.y + rect.h / 2;
		}
	}

	Rect Helper::GetExtendRect(const Rect & rect, const ClientRect & clRect)
	{
		Rect r = rect;

		r.x -= clRect.dx0;
		r.y -= clRect.dy0;
		r.w += clRect.dx0 + clRect.dx1;
		r.h += clRect.dy0 + clRect.dy1;

		return r;
	}

}

#pragma warning(pop)
