/*
*	MGUI InputManager
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MGUI_Types.h"
#include "MGUI_Widget.h"
#include "MGUI_Event.h"

namespace Rad { namespace MGUI {

	class MGUI_ENTRY InputManager : public Singleton<InputManager>
	{
	public:
		tEvent1<const MouseEvent *> E_MouseMove;
		tEvent1<const MouseEvent *> E_MouseWheel;
		tEvent1<const MouseEvent *> E_MouseDown;
		tEvent1<const MouseEvent *> E_MouseUp;

		tEvent1<const KeyEvent *> E_KeyDown;
		tEvent1<const KeyEvent *> E_KeyUp;

		tEvent1<const TouchEvent *> E_TouchDown;
		tEvent1<const TouchEvent *> E_TouchUp;
		tEvent1<const TouchEvent *> E_TouchMove;
		tEvent1<const TouchEvent *> E_TouchCancel;

		tEvent0 E_OpenIMEKeyboard;
		tEvent0 E_CloseIMEKeyboard;

	public:
		InputManager();
		~InputManager();

		void 
			SetKeyFocusedWidget(Widget * _widget, bool _notify = true);
		Widget * 
			GetKeyFocusedWidget();

		void 
			SetMouseFocusedWidget(Widget * _widget, bool _notify = true);
		Widget * 
			GetMouseFocusedWidget();

		void 
			SetForceFocus(bool b) { mForceFocus = b; }
		bool 
			GetForceFocus() { return mForceFocus; }

		bool 
			IsMouseCaptured() { return mMouseCapture; }

		bool 
			GetKeyState(int key) { return mKeyState[key]; }

		bool 
			_injectMouseMove(float _x, float _y);
		bool
			_injectMouseWheel(float _z);
		bool
			_injectMouseDown(int _id, float _x, float _y);
		bool 
			_injectMouseUp(int _id, float _x, float _y);
		bool 
			_injectKeyDown(int _key, uchar_t _text = 0);
		bool 
			_injectKeyUp(int _key);
		bool 
			_injectTouchDown(int _id, float _x, float _y);
		bool 
			_injectTouchUp(int _id, float _x, float _y);
		bool 
			_injectTouchMove(int _id, float _x, float _y);
		bool
			_injectTouchCancel(int _id, float _x, float _y);

		bool 
			SetTouchFocusWidget(int _id, Widget * _widget);
		Widget * 
			GetTouchFocusWidget(int _id);
		void 
			SetTouchClickTime(int _id, float _time);
		float 
			GetTouchClickTime(int _id);
		bool 
			IsTouchFocuesed(Widget * _widget);
		void 
			ClearTouchFocused(Widget * _widget);
		
		void 
			SetEditString(const UString & _text) { mEditString = _text; }
		const UString & 
			GetEditString() { return mEditString; }

		Float3 
			_getMousePosition() { return Float3(mMouseX, mMouseY, mMouseZ); }

	protected:
		void 
			_notifyMouseMove(float _x, float _y);
		void 
			_notifyMouseWheel(float _z);
		void 
			_notifyMouseDown(int _id, float _x, float _y);
		void 
			_notifyMouseUp(int _id, float _x, float _y);

		void 
			_notifyKeyDown(int _key, uchar_t _text);
		void 
			_notifyKeyUp(int _key);

		void 
			_notifyTouchMove(int _id, float _x, float _y);
		void 
			_notifyTouchDown(int _id, float _x, float _y);
		void 
			_notifyTouchUp(int _id, float _x, float _y);
		void 
			_notifyTouchCancel(int _id, float _x, float _y);

	protected:
		float mMouseX, mMouseY, mMouseZ;
		float mMouseDownX, mMouseDownY;
		bool mMouseCapture;
		bool mForceFocus;

		float mMouseClickTime;

		Widget* mMouseFocusWidget;
		Widget* mKeyFocusWidget;

		BitField<256> mKeyState;

		Field<Widget *, MAX_TOUCH_POINTS> mTouchFocusWidgets;
		Field<float, MAX_TOUCH_POINTS> mTouchClickTimes;

		UString mEditString;
	};


	inline void InputManager::_notifyMouseMove(float _x, float _y)
	{
		MouseEvent e(&E_MouseMove);
		e.sender = NULL;
		e.id = InputCode::MKC_LEFT;
		e.x = _x;
		e.y = _y;
		e.z = 0;
		E_MouseMove(&e);
	}

	inline void InputManager::_notifyMouseWheel(float _z)
	{
		MouseEvent e(&E_MouseWheel);
		e.sender = NULL;
		e.id = InputCode::MKC_MIDDLE;
		e.x = 0;
		e.y = 0;
		e.z = _z;
		E_MouseWheel(&e);
	}

	inline void InputManager::_notifyMouseDown(int _id, float _x, float _y)
	{
		MouseEvent e(&E_MouseDown);
		e.sender = NULL;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		e.z = 0;
		E_MouseDown(&e);
	}

	inline void InputManager::_notifyMouseUp(int _id, float _x, float _y)
	{
		MouseEvent e(&E_MouseUp);
		e.sender = NULL;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		e.z = 0;
		E_MouseUp(&e);
	}

	inline void InputManager::_notifyKeyDown(int _key, uchar_t _text)
	{
		KeyEvent e(&E_KeyDown);
		e.sender = NULL;
		e.key = _key;
		e.char_ = _text;
		E_KeyDown(&e);
	}

	inline void InputManager::_notifyKeyUp(int _key)
	{
		KeyEvent e(&E_KeyUp);
		e.sender = NULL;
		e.key = _key;
		e.char_ = 0;
		E_KeyUp(&e);
	}

	inline void InputManager::_notifyTouchMove(int _id, float _x, float _y)
	{
		TouchEvent e(&E_TouchMove);
		e.sender = NULL;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		E_TouchMove(&e);
	}

	inline void InputManager::_notifyTouchDown(int _id, float _x, float _y)
	{
		TouchEvent e(&E_TouchDown);
		e.sender = NULL;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		E_TouchDown(&e);
	}

	inline void InputManager::_notifyTouchUp(int _id, float _x, float _y)
	{
		TouchEvent e(&E_TouchUp);
		e.sender = NULL;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		E_TouchUp(&e);
	}

	inline void InputManager::_notifyTouchCancel(int _id, float _x, float _y)
	{
		TouchEvent e(&E_TouchCancel);
		e.sender = NULL;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		E_TouchCancel(&e);
	}

}}
