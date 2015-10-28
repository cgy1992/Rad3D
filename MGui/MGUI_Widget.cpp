#include "MGUI_Widget.h"
#include "MGUI_Layout.h"
#include "MGUI_Helper.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(Widget, ComponentOwner);

	Widget::Widget(const LookFeel * _lookfeel, Widget * _parent)
		: Node(_parent)
		, mLookFeel(_lookfeel)
		, mEnable(true)
		, mPickMaskEnable(false)
		, mPickFlag(ePickFlag::ALL)
		, mInheritState(false)
		, mState(eWidgetState::NORMAL)
	{
		if (_lookfeel)
		{
			Rect rect;

			rect.w = _lookfeel->GetUVRect(eWidgetState::NORMAL).w;
			rect.h = _lookfeel->GetUVRect(eWidgetState::NORMAL).h;

			SetRect(rect);
			SetClientRect(_lookfeel->GetClientRect());

			if (_lookfeel->GetSkin() != "")
			{
				mSkin = HWBufferManager::Instance()->LoadTexture(_lookfeel->GetSkin());
			}
		}
	}

	Widget::~Widget()
	{
		_notifyDestroy();

		if (InputManager::Instance()->IsTouchFocuesed(this))
			InputManager::Instance()->ClearTouchFocused(this);

		if (InputManager::Instance()->GetMouseFocusedWidget() == this)
			InputManager::Instance()->SetMouseFocusedWidget(NULL, false);

		if (InputManager::Instance()->GetKeyFocusedWidget() == this)
			InputManager::Instance()->SetKeyFocusedWidget(NULL, false);
	}

	Widget * Widget::GetChild(const FixedString32 & _name)
	{
		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			if (child->GetName() == _name)
				return child;
		}

		return NULL;
	}

	int Widget::GetChildCount()
	{
		return mChildren.Size();
	}

	Widget * Widget::GetChild(int _index)
	{
		return (Widget *)mChildren[_index];
	}

	void Widget::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		const LookFeel * _lookfeel = mLookFeel;
		if (mSkin != NULL && _lookfeel != NULL)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			Rect clipRect = mParent->GetClipRect();
			const Rect & myRect = mAbsRect;
			const Rect & clRect = mAbsClient;
			const Rect & uvRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), mSkin.c_ptr());
			const Rect & uvClientRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(mState), mSkin.c_ptr());
			const Float4 & myColor = this->GetAbsColor();

			Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, myColor, clipRect);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}
	}

	void Widget::Update(float elapsedTime)
	{
		if (mVisible)
		{
			UpdateComponent(elapsedTime);
			
			_notifyUpdate();

			OnUpdate(elapsedTime);

			OnUpdateWidgetState();

			_updateTM();

			for (int i = 0; i < mChildren.Size(); ++i)
			{
				Widget * child = GetChild(i);
				child->Update(elapsedTime);
			}
		}
	}

	void Widget::OnUpdateWidgetState()
	{
		if (!mInheritState)
		{
			mState = eWidgetState::NORMAL;

			if (!this->IsEnable())
				mState = eWidgetState::DISABLE;
			else if (this == InputManager::Instance()->GetKeyFocusedWidget())
				mState = eWidgetState::SELECT;
			else if (InputManager::Instance()->IsTouchFocuesed(this))
			{
				mState = eWidgetState::PRESS;
			}
			else if (this == InputManager::Instance()->GetMouseFocusedWidget())
			{
				if (InputManager::Instance()->IsMouseCaptured())
					mState = eWidgetState::PRESS;
				else
					mState = eWidgetState::FOCUS;
			}
		}
		else
		{
			mState = GetParent()->GetState();
		}
	}

	bool Widget::_LookupPickMask(float _x, float _y)
	{
		if (!mPickMaskEnable)
			return true;

		if (mLookFeel != NULL && mSkin != NULL)
		{
			const Rect & uvRect = mLookFeel->GetUVRect(mState);

			float u = uvRect.x * _x;
			float v = uvRect.y * _y;

			u *= mSkin->GetWidth();
			v *= mSkin->GetHeight();

			Float4 color;
			mSkin->GetColorData(color, (int)u, (int)v);

			return color.r > EPSILON_E4;
		}

		return false;
	}

	Widget * Widget::Pick(float _x, float _y)
	{
		if (!mVisible)
			return NULL;

		Rect myRect = GetAbsRect();
		if (myRect.w < 1 || myRect.h < 1)
			return NULL;

		float fx = float(_x - myRect.x) / myRect.w;
		float fy = float(_y - myRect.y) / myRect.h;

		Widget * widget = NULL;

		if (fx > 0 && fx < 1 && fy > 0 && fy < 1)
		{
			if (mPickFlag & ePickFlag::CHILD)
			{
				for (int i = mChildren.Size() - 1; i >= 0; --i)
				{
					Widget * child = GetChild(i);
					if (!child->IsVisible())
						continue;

					widget = child->Pick(_x, _y);
					if (widget != NULL)
					{
						return widget;
					}
				}
			}
			
			if ((mPickFlag & ePickFlag::SELF) && _LookupPickMask(fx, fy))
			{
				widget = this;
			}
		}

		return widget;
	}

	void Widget::SerializeXML(xml_node root, bool is_out)
	{
		if (is_out)
		{
			// Name
			if (mName != "")
			{
				root.append_node("Name").append_attribute("value", mName.c_str());
			}

			// Rect
			if (mRect != Rect())
			{
				String str; 
				str += f2str(mRect.x, true) + " ";
				str += f2str(mRect.y, true) + " ";
				str += f2str(mRect.w, true) + " ";
				str += f2str(mRect.h, true);

				root.append_node("Rect").append_attribute("value", str.c_str());
			}

			// Order
			if (mOrder != 0)
			{
				root.append_node("Order").append_attribute("value", i2str(mOrder).c_str());
			}

			// Color
			if (mColor != Float4(1, 1, 1, 1))
			{
				String str; 
				str += f2str(mColor.r, true) + " ";
				str += f2str(mColor.g, true) + " ";
				str += f2str(mColor.b, true) + " ";
				str += f2str(mColor.a, true);

				root.append_node("Color").append_attribute("value", str.c_str());
			}

			// Align
			if (mAlign != eAlign::NONE)
			{
				root.append_node("Align").append_attribute("value", i2str(mAlign._value).c_str());
			}

			// Enable
			if (!mEnable)
			{
				root.append_node("Enable").append_attribute("value", "0");
			}

			// PickFlag
			if (mPickFlag != ePickFlag::ALL)
			{
				root.append_node("mPickFlag").append_attribute("value", i2str(mPickFlag).c_str());
			}

			// InheritAlpha
			if (!mInheritAlpha)
			{
				root.append_node("InheritAlpha").append_attribute("value", "0");
			}
		}
		else
		{// Name
			xml_node node = root.first_node("Name");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetName(str);
			}

			// Rect
			node = root.first_node("Rect");
			if (node != NULL)
			{
				Rect rect;
				const char * str = node.first_attribute("value");
				str = str_getfloat(rect.x, str);
				str = str_getfloat(rect.y, str);
				str = str_getfloat(rect.w, str);
				str = str_getfloat(rect.h, str);

				SetRect(rect);
			}

			// Order
			node = root.first_node("Order");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetOrder(atoi(str));
			}

			// Color
			node = root.first_node("Color");
			if (node != NULL)
			{
				Float4 color = Float4(1, 1, 1, 1);
				const char * str = node.first_attribute("value");
				str = str_getfloat(color.r, str);
				str = str_getfloat(color.g, str);
				str = str_getfloat(color.b, str);
				str = str_getfloat(color.a, str);

				SetColor(color);
			}

			// Align
			node = root.first_node("Align");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetAlign(atoi(str));
			}

			// Enable
			node = root.first_node("Enable");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetEnable(atoi(str) != 0);
			}

			// PickFlag
			node = root.first_node("PickFlag");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetPickFlag(atoi(str));
			}

			// InheritAlpha
			node = root.first_node("InheritAlpha");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetInheritAlpha(atoi(str) != 0);
			}
		}
	}

}}
