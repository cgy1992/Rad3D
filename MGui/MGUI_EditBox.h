/*
*	EditBox
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Widget.h"
#include "MGUI_TextBox.h"
#include "MGUI_Timer.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY EditBox : public Widget
	{
		DECLARE_RTTI();

	public:
		tEvent2<const Event *, const UString &> E_TextChanged;
		tEvent1<const Event *> E_TextReturn;

	public:
		EditBox(const LookFeel * _lookfeel, Widget * _parent);
		virtual ~EditBox();

		void 
			SetCaption(const UString & caption, bool _sendEvent = true);
		const UString & 
			GetCaption() const;

		void 
			SetFontName(const FixedString32 & name);
		FixedString32
			GetFontName() const;
		Font *
			_getFont() { return mTextBox->GetFont(); }

		void 
			SetPasswordEnable(bool enable);
		bool 
			GetPasswordEnable() const;

		void 
			SetTextColor(const Float4 & color);
		const Float4 & 
			GetTextColor() const;

		void 
			SetTextAlign(eAlign align);
		eAlign 
			GetTextAlign() const;

		void 
			SetStatic(bool _static);
		bool 
			GetStatic() { return mStatic; }

		void 
			SetTextLimit(int count) { mTextLimit = count; }
		int 
			GetTextLimit() { return mTextLimit; }

		void
			SetTranslation(bool b) { mTranslation = b; }
		bool
			GetTranslation() const { return mTranslation; }

		void 
			SetSelectCursorColor(const Float4 & _color) { mSelectCursorColor = _color; }
		const Float4 & 
			GetSelectCursorColor() { return mSelectCursorColor; }

		void 
			SetSelectBackColor(const Float4 & _color) { mSelectBackColor = _color; }
		const Float4 & 
			GetSelectBackColor() { return mSelectBackColor; }

		virtual bool 
			CanKeyFoucsed() { return !mStatic; }

		virtual void 
			UpdateRenderItem(Layout * _layout);

		virtual void
			SerializeXML(xml_node root, bool is_out);

	protected:
		virtual void 
			OnMouseDown(int _id, float _x, float _y);
		virtual void 
			OnMouseDrag(float _x, float _y);

		virtual void 
			OnTouchDown(int _id, float _x, float _y);

		virtual void 
			OnKeyLostFocus(Widget * _new);
		virtual void 
			OnKeySetFocus(Widget* _old);
		virtual void 
			OnKeyDown(int _key, uchar_t _char);

		void 
			OnTimer(Component *);

	protected:
		TextBox * mTextBox;

		bool mTranslation;
		bool mStatic;
		int mTextLimit;
		Float4 mSelectCursorColor;
		Float4 mSelectBackColor;

		int mSelectIndex;
		int mSelectStartIndex;
		int mSelectEndIndex;

		bool mSelectVisible;
		Timer * mTimer;

	};

}}
