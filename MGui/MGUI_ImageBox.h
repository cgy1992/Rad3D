/*
*	Image
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY ImageBox : public Widget
	{
		DECLARE_RTTI();

	public:
		ImageBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~ImageBox();

		void 
			SetSkin(const String & _skin);
		void 
			SetSkinEx(TexturePtr _skin);
		const String &
			GetSkinName() const;

		void 
			SetSkinAligned(const String & _skin, const Rect & _uv = Rect(0, 0, 1, 1));
		void 
			SetSkinAlignedEx(TexturePtr _skin, const Rect & _uv = Rect(0, 0, 1, 1));

		void 
			SetUVRect(const Rect & _uv);
		const Rect & 
			GetUVRect() const { return mUVRect; }

		virtual void 
			UpdateRenderItem(Layout * _layout);

		virtual void
			SerializeXML(xml_node root, bool is_out);

	protected:
		virtual bool 
			_LookupPickMask(float _x, float y);

	protected:
		String mSkinName;
		Rect mUVRect;
	};

}}
