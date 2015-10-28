/*
*	Widget Input
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Event.h"

namespace Rad { namespace MGUI {

	class Widget;

	class MGUI_ENTRY WidgetInput
	{
	public:
		tEvent1<const FocusEvent *> E_MouseSetFocus;
		tEvent1<const FocusEvent *> E_MouseLostFocus;
		tEvent1<const MouseEvent *> E_MouseDrag;
		tEvent1<const MouseEvent *> E_MouseMove;
		tEvent1<const MouseEvent *> E_MouseWheel;
		tEvent1<const MouseEvent *> E_MouseDown;
		tEvent1<const MouseEvent *> E_MouseUp;
		tEvent1<const MouseEvent *> E_MouseClick;
		tEvent1<const MouseEvent *> E_MouseDoubleClick;

		tEvent1<const FocusEvent *> E_KeySetFocus;
		tEvent1<const FocusEvent *> E_KeyLostFocus;
		tEvent1<const KeyEvent *> E_KeyDown;
		tEvent1<const KeyEvent *> E_KeyUp;

		tEvent1<const TouchEvent *> E_TouchDown;
		tEvent1<const TouchEvent *> E_TouchUp;
		tEvent1<const TouchEvent *> E_TouchClick;
		tEvent1<const TouchEvent *> E_TouchDoubleClick;
		tEvent1<const TouchEvent *> E_TouchMove;
		tEvent1<const TouchEvent *> E_TouchCancel;

		tEvent1<const ClickEvent *> E_Click;
		tEvent1<const ClickEvent *> E_DoubleClick;

		tEvent1<const Event *> E_Update;
		tEvent1<const Event *> E_Destroy;

	public:
		WidgetInput() {}
		virtual ~WidgetInput() {}

		void 
			_notifyMouseLostFocus(Widget * _new);
		void 
			_notifyMouseSetFocus(Widget * _old);
		void 
			_notifyMouseDrag(float _x, float _y);
		void 
			_notifyMouseMove(float _x, float _y);
		void 
			_notifyMouseWheel(float _dz);
		void 
			_notifyMouseDown(int _id, float _x, float _y);
		void 
			_notifyMouseUp(int _id, float _x, float _y);
		void 
			_notifyMouseClick(float _x, float _y);
		void 
			_notifyMouseDoubleClick(float _x, float _y);
		void 
			_notifyKeyLostFocus(Widget* _new);
		void 
			_notifyKeySetFocus(Widget* _old);
		void 
			_notifyKeyDown(int _key, uchar_t _char);
		void 
			_notifyKeyUp(int _key);

		//
		void 
			_notifyTouchDown(int id, float x, float y);
		void 
			_notifyTouchUp(int id, float x, float y);
		void 
			_notifyTouchClick(int id, float x, float y);
		void 
			_notifyTouchDoubleClick(int id, float x, float y);
		void 
			_notifyTouchMove(int id, float x, float y);
		void 
			_notifyTouchCancel(int id, float x, float y);

		//
		void 
			_notifyClick(int flag, float x, float y);
		void 
			_notifyDoubleClick(int flag, float x, float y);

		//
		void 
			_notifyUpdate();
		void 
			_notifyDestroy();

	protected:
		virtual void 
			OnMouseLostFocus(Widget* _new) {}
		virtual void 
			OnMouseSetFocus(Widget* _old) {}
		virtual void 
			OnMouseDrag(float _x, float _y) {}
		virtual void 
			OnMouseMove(float _x, float _y) {}
		virtual void 
			OnMouseWheel(float _dz) {}
		virtual void 
			OnMouseDown(int _id, float _x, float _y) {}
		virtual void 
			OnMouseUp(int _id, float _x, float _y) {}
		virtual void 
			OnMouseClick() {}
		virtual void 
			OnMouseDoubleClick() {}

		virtual void 
			OnKeyLostFocus(Widget* _new) {}
		virtual void 
			OnKeySetFocus(Widget* _old) {}
		virtual void 
			OnKeyDown(int _key, uchar_t _char) {}
		virtual void 
			OnKeyUp(int _key) {}

		virtual void 
			OnTouchDown(int id, float x, float y) {}
		virtual void 
			OnTouchUp(int id, float x, float y) {}
		virtual void 
			OnTouchClick(int id, float x, float y) {}
		virtual void 
			OnTouchDoubleClick(int id, float x, float y) {}
		virtual void 
			OnTouchMove(int id, float x, float y) {}
		virtual void 
			OnTouchCancel(int id, float x, float y) {}

		virtual void 
			OnClick() {}
		virtual void 
			OnDoubleClick() {}
	};

	
}}