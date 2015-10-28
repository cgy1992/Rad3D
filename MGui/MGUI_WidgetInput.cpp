#include "MGUI_WidgetInput.h"
#include "MGUI_Widget.h"

namespace Rad { namespace MGUI {

	void WidgetInput::_notifyMouseLostFocus(Widget * _new)
	{
		OnMouseLostFocus(_new);

		FocusEvent e(&E_MouseLostFocus);
		e.sender = (Widget *)this;
		e.widget = _new;

		E_MouseLostFocus(&e);
	}

	void WidgetInput::_notifyMouseSetFocus(Widget * _old)
	{
		OnMouseSetFocus(_old);

		FocusEvent e(&E_MouseSetFocus);
		e.sender = (Widget *)this;
		e.widget = _old;

		E_MouseSetFocus(&e);
	}

	void WidgetInput::_notifyMouseDrag(float _x, float _y)
	{
		OnMouseDrag(_x, _y);

		MouseEvent e(&E_MouseDrag);
		e.sender = (Widget *)this;
		e.id = InputCode::MKC_LEFT;
		e.x = _x;
		e.y = _y;
		e.z = 0;

		E_MouseDrag(&e);
	}

	void WidgetInput::_notifyMouseMove(float _x, float _y)
	{
		OnMouseMove(_x, _y);

		MouseEvent e(&E_MouseMove);
		e.sender = (Widget *)this;
		e.id = InputCode::MKC_NONE;
		e.x = _x;
		e.y = _y;
		e.z = 0;

		E_MouseMove(&e);
	}

	void WidgetInput::_notifyMouseWheel(float _z)
	{
		OnMouseWheel(_z);

		MouseEvent e(&E_MouseWheel);
		e.sender = (Widget *)this;
		e.id = InputCode::MKC_MIDDLE;
		e.x = 0;
		e.y = 0;
		e.z = _z;

		E_MouseWheel(&e);
	}

	void WidgetInput::_notifyMouseDown(int _id, float _x, float _y)
	{
		OnMouseDown(_id, _x, _y);

		MouseEvent e(&E_MouseDown);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		e.z = 0;

		E_MouseDown(&e);
	}

	void WidgetInput::_notifyMouseUp(int _id, float _x, float _y)
	{
		OnMouseUp(_id, _x, _y);

		MouseEvent e(&E_MouseUp);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;
		e.z = 0;

		E_MouseUp(&e);
	}

	void WidgetInput::_notifyMouseClick(float _x, float _y)
	{
		OnMouseClick();

		MouseEvent e(&E_MouseClick);
		e.sender = (Widget *)this;
		e.id = InputCode::MKC_LEFT;
		e.x = _x;
		e.y = _y;
		e.z = 0;

		E_MouseClick(&e);

		_notifyClick(0, _x, _y);
	}

	void WidgetInput::_notifyMouseDoubleClick(float _x, float _y)
	{
		OnMouseDoubleClick();

		MouseEvent e(&E_MouseDoubleClick);
		e.sender = (Widget *)this;
		e.id = InputCode::MKC_LEFT;
		e.x = _x;
		e.y = _y;
		e.z = 0;

		E_MouseDoubleClick(&e);

		_notifyDoubleClick(0, _x, _y);
	}

	void WidgetInput::_notifyKeyLostFocus(Widget * _new)
	{
		OnKeyLostFocus(_new);

		FocusEvent e(&E_KeyLostFocus);
		e.sender = (Widget *)this;
		e.widget = _new;

		E_KeyLostFocus(&e);
	}

	void WidgetInput::_notifyKeySetFocus(Widget * _old)
	{
		OnKeySetFocus(_old);

		FocusEvent e(&E_KeySetFocus);
		e.sender = (Widget *)this;
		e.widget = _old;

		E_KeySetFocus(&e);
	}

	void WidgetInput::_notifyKeyDown(int _key, uchar_t _char)
	{
		OnKeyDown(_key, _char);

		KeyEvent e(&E_KeyDown);
		e.sender = (Widget *)this;
		e.key = _key;
		e.char_ = _char;

		E_KeyDown(&e);
	}

	void WidgetInput::_notifyKeyUp(int _key)
	{
		OnKeyUp(_key);

		KeyEvent e(&E_KeyUp);
		e.sender = (Widget *)this;
		e.key = _key;
		e.char_ = 0;

		E_KeyUp(&e);
	}

	void WidgetInput::_notifyTouchDown(int _id, float _x, float _y)
	{
		OnTouchDown(_id, _x, _y);

		TouchEvent e(&E_TouchDown);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;

		E_TouchDown(&e);
	}

	void WidgetInput::_notifyTouchUp(int _id, float _x, float _y)
	{
		OnTouchUp(_id, _x, _y);

		TouchEvent e(&E_TouchUp);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;

		E_TouchUp(&e);
	}

	void WidgetInput::_notifyTouchClick(int _id, float _x, float _y)
	{
		OnTouchClick(_id, _x, _y);

		TouchEvent e(&E_TouchClick);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;

		E_TouchClick(&e);

		_notifyClick(1, _x, _y);
	}

	void WidgetInput::_notifyTouchDoubleClick(int _id, float _x, float _y)
	{
		OnTouchDoubleClick(_id, _x, _y);

		TouchEvent e(&E_TouchDoubleClick);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;

		E_TouchDoubleClick(&e);

		_notifyDoubleClick(1, _x, _y);
	}


	void WidgetInput::_notifyTouchMove(int _id, float _x, float _y)
	{
		OnTouchMove(_id, _x, _y);

		TouchEvent e(&E_TouchMove);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;

		E_TouchMove(&e);
	}

	void WidgetInput::_notifyTouchCancel(int _id, float _x, float _y)
	{
		OnTouchCancel(_id, _x, _y);

		TouchEvent e(&E_TouchCancel);
		e.sender = (Widget *)this;
		e.id = _id;
		e.x = _x;
		e.y = _y;

		E_TouchCancel(&e);
	}

	void WidgetInput::_notifyClick(int flag, float x, float y)
	{
		OnClick();

		ClickEvent e(&E_Click);
		e.sender = (Widget *)this;
		e.flag = flag;
		e.x = x;
		e.y = y;

		E_Click(&e);
	}

	void WidgetInput::_notifyDoubleClick(int flag, float x, float y)
	{
		OnDoubleClick();

		ClickEvent e(&E_DoubleClick);
		e.sender = (Widget *)this;
		e.flag = flag;
		e.x = x;
		e.y = y;

		E_DoubleClick(&e);
	}

	void WidgetInput::_notifyUpdate()
	{
		Event e(&E_Update);
		e.sender = (Widget *)this;

		E_Update(&e);
	}

	void WidgetInput::_notifyDestroy()
	{
		Event e(&E_Destroy);
		e.sender = (Widget *)this;

		E_Destroy(&e);
	}

}}