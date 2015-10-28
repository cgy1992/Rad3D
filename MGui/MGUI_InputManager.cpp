#include "MGUI_InputManager.h"
#include "MGUI_Engine.h"
#include "MRoot.h"

#define INPUT_TIME_DOUBLE_CLICK 0.25f

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(InputManager);

	InputManager::InputManager()
	{
		mMouseX = mMouseY = mMouseZ = 0;
		mMouseCapture = false;
		mForceFocus = false;
		mMouseFocusWidget = NULL;
		mKeyFocusWidget = NULL;

		mMouseClickTime = 0;

		mKeyState.ClearAll();

		for (int i = 0; i < MAX_TOUCH_POINTS; ++i)
		{
			mTouchFocusWidgets[i] = NULL;
			mTouchClickTimes[i] = 0;
		}
	}

	InputManager::~InputManager()
	{
	}

	bool InputManager::_injectMouseMove(float _x, float _y)
	{
		_notifyMouseMove(_x, _y);

		mMouseX = _x;
		mMouseY = _y;

		if (mMouseCapture)
		{
			if (mMouseFocusWidget != NULL)
				mMouseFocusWidget->_notifyMouseDrag(_x, _y);
			else
				mMouseCapture = false;

			return true;
		}

		Widget * widget = mMouseFocusWidget;

		if (!mForceFocus)
			widget = Engine::Instance()->GetWidget(_x, _y);

		if (widget == mMouseFocusWidget && widget != NULL && widget->IsEnable())
			mMouseFocusWidget->_notifyMouseMove(_x, _y);
		else
			SetMouseFocusedWidget(widget);

		return mMouseFocusWidget != NULL;
	}

	bool InputManager::_injectMouseWheel(float _z)
	{
		_notifyMouseWheel(_z);

		float dz = _z - mMouseZ;
		mMouseZ = _z;

		if (mMouseFocusWidget != NULL)
		{
			mMouseFocusWidget->_notifyMouseWheel(dz);

			return false;
		}

		return true;
	}

	bool InputManager::_injectMouseDown(int _id, float _x, float _y)
	{
		_notifyMouseDown(_id, _x, _y);

		if (mMouseCapture)
		{
			Widget * widget = Engine::Instance()->GetWidget(_x, _y);

			if (widget != mMouseFocusWidget)
			{
				SetMouseFocusedWidget(widget);
			}

			mMouseCapture = false;
		}

		if (mMouseFocusWidget == NULL)
		{
			SetKeyFocusedWidget(NULL);

			return false;
		}
		else if (!mMouseFocusWidget->IsEnable())
		{
			return true;
		}

		if (mMouseFocusWidget)
		{
			if (_id == InputCode::MKC_LEFT)
			{
				mMouseCapture = true;
			}

			SetKeyFocusedWidget(mMouseFocusWidget);

			if (mMouseFocusWidget)
			{
				mMouseFocusWidget->_notifyMouseDown(_id, _x, _y);
			}
		}

		return true;
	}

	bool InputManager::_injectMouseUp(int _id, float _x, float _y)
	{
		_notifyMouseUp(_id, _x, _y);

		if (_id == InputCode::MKC_LEFT)
			mMouseCapture = false;

		if (mMouseFocusWidget)
		{
			if (!mMouseFocusWidget->IsEnable())
				return true;

			mMouseFocusWidget->_notifyMouseUp(_id, _x, _y);

			if (mMouseFocusWidget != NULL)
			{
				if (InputCode::MKC_LEFT == _id)
				{
					if (Root::Instance()->GetTime() - mMouseClickTime < INPUT_TIME_DOUBLE_CLICK)
					{
						mMouseFocusWidget->_notifyMouseClick(_x, _y);

						if (mMouseFocusWidget != NULL)
							mMouseFocusWidget->_notifyMouseDoubleClick(_x, _y);

						mMouseClickTime = 0;
					}
					else
					{
						Widget* item = Engine::Instance()->GetWidget(_x, _y);

						if (item == mMouseFocusWidget)
						{
							mMouseFocusWidget->_notifyMouseClick(_x, _y);
						}

						mMouseClickTime = Root::Instance()->GetTime();
					}
				}
			}

			_injectMouseMove(_x, _y);

			return true;
		}

		return false;
	}

	bool InputManager::_injectKeyDown(int _key, uchar_t _text)
	{
		_notifyKeyDown(_key, _text);

		mKeyState.Set(_key);

		if (mKeyFocusWidget != NULL)
			mKeyFocusWidget->_notifyKeyDown(_key, _text);

		return mKeyFocusWidget != NULL;
	}

	bool InputManager::_injectKeyUp(int _key)
	{
		_notifyKeyUp(_key);

		mKeyState.Clear(_key);

		if (mKeyFocusWidget != NULL)
			mKeyFocusWidget->_notifyKeyUp(_key);

		return mKeyFocusWidget != NULL;
	}

	void InputManager::SetKeyFocusedWidget(Widget * _widget, bool _notify)
	{
		if (_widget && !_widget->CanKeyFoucsed() && _widget->IsEnable())
			_widget = NULL;

		if (mKeyFocusWidget != _widget)
		{
			Widget * oldWidget = mKeyFocusWidget;

			mKeyFocusWidget = _widget;

			if (_notify)
			{
				if (oldWidget != NULL)
					oldWidget->_notifyKeyLostFocus(_widget);

				if (_widget != NULL)
					_widget->_notifyKeySetFocus(oldWidget);
			}

			if (_widget == NULL)
				E_CloseIMEKeyboard();
			else
				E_OpenIMEKeyboard();
		}
	}

	Widget * InputManager::GetKeyFocusedWidget()
	{
		return mKeyFocusWidget;
	}

	void InputManager::SetMouseFocusedWidget(Widget * _widget, bool _notify)
	{
		if (mMouseFocusWidget != _widget)
		{
			Widget * oldWidget = mMouseFocusWidget ;

			mMouseFocusWidget  = _widget;

			if (_notify)
			{
				if (oldWidget != NULL)
					oldWidget->_notifyMouseLostFocus(_widget);

				if (_widget != NULL)
					_widget->_notifyMouseSetFocus(oldWidget);
			}
		}
	}

	Widget * InputManager::GetMouseFocusedWidget()
	{
		return mMouseFocusWidget;
	}

	bool InputManager::_injectTouchDown(int _id, float _x, float _y)
	{
		_notifyTouchDown(_id, _x, _y);

		Widget * widget = Engine::Instance()->GetWidget(_x, _y);

		if (SetTouchFocusWidget(_id, widget) && widget != NULL)
		{
			widget->_notifyTouchDown(_id, _x, _y);

			if (widget->CanKeyFoucsed())
			{
				SetKeyFocusedWidget(widget);
			}
		}

		return widget != NULL;
	}

	bool InputManager::_injectTouchUp(int _id, float _x, float _y)
	{
		_notifyTouchUp(_id, _x, _y);

		Widget * c_widget = GetTouchFocusWidget(_id);

		if (c_widget != NULL)
		{
			c_widget->_notifyTouchUp(_id, _x, _y);

			Widget* item = Engine::Instance()->GetWidget(_x, _y);

			if (item == c_widget)
			{
				item->_notifyTouchClick(_id, _x, _y);

				if (Root::Instance()->GetTime() - GetTouchClickTime(_id) < INPUT_TIME_DOUBLE_CLICK)
				{
					item->_notifyTouchDoubleClick(_id, _x, _y);
					SetTouchClickTime(_id, 0);
				}
				else
				{
					SetTouchClickTime(_id, Root::Instance()->GetTime());
				}
			}

			SetTouchFocusWidget(_id, NULL);
		}

		return c_widget != NULL;
	}

	bool InputManager::_injectTouchMove(int _id, float _x, float _y)
	{
		_notifyTouchMove(_id, _x, _y);

		Widget * c_widget = GetTouchFocusWidget(_id);

		if (c_widget)
			c_widget->_notifyTouchMove(_id, _x, _y);

		return c_widget != NULL;
	}

	bool InputManager::_injectTouchCancel(int _id, float _x, float _y)
	{
		_notifyTouchCancel(_id, _x, _y);

		Widget * c_widget = GetTouchFocusWidget(_id);

		if (c_widget)
		{
			c_widget->_notifyTouchCancel(_id, _x, _y);
			SetTouchFocusWidget(_id, NULL);
		}

		return c_widget != NULL;
	}

	bool InputManager::SetTouchFocusWidget(int _id, Widget * _widget)
	{
		if (_id >= MAX_TOUCH_POINTS)
			return false;

		mTouchFocusWidgets[_id] = _widget;

		return true;
	}

	Widget * InputManager::GetTouchFocusWidget(int _id)
	{
		if (_id >= MAX_TOUCH_POINTS)
			return NULL;

		return mTouchFocusWidgets[_id];
	}


	void InputManager::SetTouchClickTime(int _id, float _time)
	{
		if (_id >= MAX_TOUCH_POINTS)
			return ;

		mTouchClickTimes[_id] = _time;
	}

	float InputManager::GetTouchClickTime(int _id)
	{
		if (_id >= MAX_TOUCH_POINTS)
			return 0;

		return mTouchClickTimes[_id];
	}

	
	bool InputManager::IsTouchFocuesed(Widget * _widget)
	{
		for (int i = 0; i < mTouchFocusWidgets.Size(); ++i)
		{
			if (mTouchFocusWidgets[i] == _widget)
				return true;
		}

		return false;
	}

	void InputManager::ClearTouchFocused(Widget * _widget)
	{
		for (int i = 0; i < MAX_TOUCH_POINTS; ++i)
		{
			mTouchFocusWidgets[i] = NULL;
		}
	}

}