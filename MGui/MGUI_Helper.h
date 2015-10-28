/*
*	MGUI Helper
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Entry.h"
#include "MGUI_Types.h"

#include "MGUI_LookFeel.h"
#include "MGUI_Widget.h"
#include "MGUI_RenderItem.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY Helper : public Singleton<Helper>
	{
	public:
		Helper();
		~Helper();

		Rect 
			MapUVRect(const Rect & _rect, Texture * _skin);

		bool 
			Clip(Rect & _orect, Rect & _ouv, const Rect & _rect, const Rect & _uv, const Rect & _clipRect);

		const Vertex * 
			GetQuad(const Rect & _rect, const Rect & _uv, const Float4 & _c);

		void 
			AddRenderItem(RenderItem * ri, const Rect & myRect, const Rect & clRect,
			const Rect & uvRect, const Rect & uvClientRect, const Float4 & color, const Rect & clipRect);

		void 
			SetUnknownChar(uchar_t _char) { mUnknownChar = _char; }
		uchar_t 
			GetUnknownChar() { return mUnknownChar; }

		TexturePtr 
			GetWhiteTexture() { return RenderHelper::Instance()->GetWhiteTexture(); }
		TexturePtr 
			GetBlackTexture() { return RenderHelper::Instance()->GetBlackTexture(); }

		Rect 
			GetAlignRect(const Rect & rect, const Rect & parent, eAlign align);
		Rect 
			GetRelativeRect(Widget * widget, const Rect & absRect);
		Rect 
			GetAbsolutelyRect(Widget * widget, const Rect & relRect);
		Rect 
			GetExtendRect(const Rect & rect, const ClientRect & clRect);

		void 
			GetAlignPoint(float & x, float & y, const Rect & rect, eAlign align);

	protected:
		uchar_t mUnknownChar;
	};

}}
