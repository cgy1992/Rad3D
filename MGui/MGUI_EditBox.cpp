#include "MGUI_EditBox.h"
#include "MGUI_Engine.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(EditBox, Widget);

	EditBox::EditBox(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mSelectBackColor = Float4(0.4f, 0.4f, 0.75f);
		mSelectCursorColor = Float4(1, 1, 1);

		mSelectIndex = mSelectStartIndex = mSelectEndIndex = 0;

		mStatic = false;

		mSelectVisible = false;
		mTimer = new Timer;
		mTimer->SetTime(0.5f, 0);
		mTimer->SetEnable(false);
		mTimer->E_Timer += new cListener1<EditBox, Component *>(this, &EditBox::OnTimer);
		AddComponent(mTimer);

		mTextBox = new TextBox(_lookfeel, this);
		mTextBox->SetAlign(eAlign::LEFT | eAlign::V_CENTER);
		mTextBox->SetPickFlag(ePickFlag::NONE);
		mTextBox->SetInheritState(true);

		mTextLimit = 128;
	}

	EditBox::~EditBox()
	{
	}

	void EditBox::SetCaption(const UString & caption, bool _sendEvent)
	{
		mTextBox->SetCaption(caption);

		mSelectIndex = Min(caption.Length(), mSelectIndex);
		mSelectStartIndex = mSelectEndIndex = mSelectIndex;

		if (_sendEvent)
		{
			Event e(&E_TextChanged);
			e.sender = this;
			E_TextChanged(&e, caption);
		}
	}

	const UString & EditBox::GetCaption() const
	{
		return mTextBox->GetCaption();
	}

	void EditBox::SetFontName(const FixedString32 & name)
	{
		Font * p = FontManager::Instance()->GetFont(name);
		if (p)
		{
			mTextBox->SetFont(p);
		}
	}

	FixedString32 EditBox::GetFontName() const
	{
		Font * font = mTextBox->GetFont();
		if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
		{
			return font->GetName();
		}

		return "";
	}

	void EditBox::SetPasswordEnable(bool enable)
	{
		mTextBox->SetPasswordEnable(enable);
	}

	bool EditBox::GetPasswordEnable() const
	{
		return mTextBox->GetPasswordEnable();
	}

	void EditBox::SetTextColor(const Float4 & color)
	{
		mTextBox->SetColor(color);
	}

	const Float4 & EditBox::GetTextColor() const
	{
		return mTextBox->GetColor();
	}

	void EditBox::SetTextAlign(eAlign _align)
	{
		eAlign a = eAlign::LEFT;

		if (_align._value & eAlign::LEFT)
		{
			a = eAlign::LEFT;
		}
		else if (_align._value & eAlign::H_CENTER)
		{
			a = eAlign::H_CENTER;
		}
		else if (_align._value & eAlign::RIGHT)
		{
			a = eAlign::RIGHT;
		}

		mTextBox->SetAlign(a._value | eAlign::V_CENTER);
	}

	eAlign EditBox::GetTextAlign() const
	{
		eAlign a = eAlign::LEFT;
		eAlign _align = mTextBox->GetAlign();

		if (_align._value & eAlign::LEFT)
		{
			a = eAlign::LEFT;
		}
		else if (_align._value & eAlign::H_CENTER)
		{
			a = eAlign::H_CENTER;
		}
		else if (_align._value & eAlign::RIGHT)
		{
			a = eAlign::RIGHT;
		}

		return a;
	}

	void EditBox::SetStatic(bool _static)
	{
		mStatic = _static;

		if (InputManager::Instance()->GetKeyFocusedWidget() == this)
			InputManager::Instance()->SetKeyFocusedWidget(NULL);
	}

	void EditBox::OnMouseDown(int _id, float _x, float _y)
	{
		if (mStatic)
			return ;

		mSelectStartIndex = mSelectEndIndex = 0;

		int selectIndex = mTextBox->_mapTextIndex(_x, _y);

		mSelectIndex = mSelectStartIndex = mSelectEndIndex = Max(selectIndex, 0);

		InputManager::Instance()->E_OpenIMEKeyboard();
	}

	void EditBox::OnMouseDrag(float _x, float _y)
	{
		if (!mStatic)
		{
			int selectIndex = mTextBox->_mapTextIndex(_x, _y);

			mSelectStartIndex = Min(mSelectIndex, selectIndex);
			mSelectEndIndex = Max(mSelectIndex, selectIndex);
		}
	}

	void EditBox::OnTouchDown(int _id, float _x, float _y)
	{
		OnMouseDown(InputCode::MKC_LEFT, _x, _y);
	}

	void EditBox::OnKeyLostFocus(Widget * _new)
	{
		mSelectVisible = false;
		mTimer->SetEnable(false);
	}

	void EditBox::OnKeySetFocus(Widget* _old)
	{
		if (mEnable && !mStatic)
		{
			mSelectVisible = true;
			mTimer->SetEnable(true);

			InputManager::Instance()->SetEditString(GetCaption());
		}
	}

	void EditBox::OnKeyDown(int _key, uchar_t _char)
	{
		if (!mEnable || mStatic)
			return ;

		UString caption = mTextBox->GetCaption();

		if (_key == InputCode::KC_ESCAPE)
			InputManager::Instance()->SetKeyFocusedWidget(NULL);
		
		else if (_key == InputCode::KC_BACK ||
				 _key == InputCode::KC_DELETE)
		{
			if (mSelectEndIndex == 0)
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				caption.Erase(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				mTextBox->SetCaption(caption);
			}
			else
			{
				caption.Erase(mSelectIndex - 1, 1);
				mSelectIndex = mSelectIndex - 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				mTextBox->SetCaption(caption);
			}
		}

		else if (_key == InputCode::KC_LEFTARROW)
		{
			if (mSelectEndIndex == 0)
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
			else
			{
				mSelectIndex = mSelectIndex - 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
		}

		else if (_key == InputCode::KC_RIGHTARROW)
		{
			if (mSelectEndIndex == caption.Length())
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				mSelectIndex = mSelectEndIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
			else
			{
				mSelectIndex = Min(mSelectIndex + 1, caption.Length());
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}
		}

		else if (_key == InputCode::KC_NUMPADENTER ||
			     _key == InputCode::KC_ENTER)
		{
			Event e(&E_TextChanged);
			e.sender = this;
			E_TextReturn(&e);

			InputManager::Instance()->SetKeyFocusedWidget(NULL);
		}

		else if (InputManager::Instance()->GetKeyState(InputCode::KC_LCONTROL))
		{
			if (_key == InputCode::KC_C)
			{
				UString text;

				for (int i = mSelectStartIndex; i < mSelectEndIndex; ++i)
				{
					text += caption[i];
				}

				if (text.Length() > 0)
					Clipboard::Instance()->SetText(text);
			}
			else if (_key == InputCode::KC_V)
			{
				UString text = Clipboard::Instance()->GetText();

				if (text.Length() > 0)
				{
					if (mSelectEndIndex > mSelectStartIndex)
					{
						caption.Erase(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

						mSelectIndex = mSelectStartIndex;
						mSelectStartIndex = mSelectEndIndex = mSelectIndex;
					}

					caption.Insert(mSelectIndex, text.c_str(), text.Length());

					if (caption.Length() <= mTextLimit)
					{
						mSelectIndex += text.Length();
						mSelectStartIndex = mSelectEndIndex = mSelectIndex;

						mTextBox->SetCaption(caption);
					}
				}
			}
			else if (_key == InputCode::KC_X)
			{
				UString text;

				for (int i = mSelectStartIndex; i < mSelectEndIndex; ++i)
				{
					text += caption[i];
				}

				if (mSelectEndIndex > mSelectStartIndex)
				{
					caption.Erase(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

					mSelectIndex = mSelectStartIndex;
					mSelectStartIndex = mSelectEndIndex = mSelectIndex;

					mTextBox->SetCaption(caption);
				}

				if (text.Length() > 0)
					Clipboard::Instance()->SetText(text);
			}
		}

		else if (_char != 0)
		{
			if (mSelectEndIndex > mSelectStartIndex)
			{
				caption.Erase(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}

			caption.Insert(mSelectIndex, _char);

			if (caption.Length() <= mTextLimit)
			{
				mSelectIndex += 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				mTextBox->SetCaption(caption);
			}
		}
	}

	void EditBox::OnTimer(Component *)
	{
		mSelectVisible = !mSelectVisible;
	}

	void EditBox::UpdateRenderItem(Layout * _layout)
	{
		if (!mVisible)
			return ;

		const LookFeel * _lookfeel = mLookFeel;
		if (_lookfeel)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder(), mSkin.c_ptr());

			Rect clipRect = mParent->GetClipRect();
			const Rect & myRect = mAbsRect;
			const Rect & clRect = mAbsClient;
			const Rect & uvRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVRect(mState), mSkin.c_ptr());
			const Rect & uvClientRect = Helper::Instance()->MapUVRect(_lookfeel->GetUVClientRect(mState), mSkin.c_ptr());

			Helper::Instance()->AddRenderItem(ri, myRect, clRect, uvRect, uvClientRect, mAbsColor, clipRect);
		}

		if (mTextBox->GetFont() == NULL)
			return ;

		if (mSelectEndIndex > mSelectStartIndex && mState == eWidgetState::SELECT)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder() + eOrder::TEXT_BACK, Helper::Instance()->GetWhiteTexture().c_ptr());

			mTextBox->_updateSelectedRenderItem(ri, mSelectBackColor, mSelectStartIndex, mSelectEndIndex);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}

		if (mSelectStartIndex == mSelectEndIndex && mSelectVisible)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder() + eOrder::TEXT_CURSOR, Helper::Instance()->GetWhiteTexture().c_ptr());

			mTextBox->_updateSelectorRenderItem(ri, mSelectCursorColor, mSelectIndex);
		}
	}

	void EditBox::SerializeXML(xml_node root, bool is_out)
	{
		Widget::SerializeXML(root, is_out);

		if (is_out)
		{
			// Caption
			if (GetCaption().Length() > 0)
			{
				String caption;

				caption.FromUnicode(GetCaption().c_str());

				root.append_node("Caption").append_attribute("value", caption.c_str());
			}

			// Font
			Font * font = _getFont();
			if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
			{
				root.append_node("Font").append_attribute("value", GetFontName().c_str());
			}

			// TextColor
			if (GetTextColor() != Float4(1, 1, 1, 1))
			{
				const Float4 & color = GetTextColor();

				String str;
				str += f2str(color.r, true) + " ";
				str += f2str(color.g, true) + " ";
				str += f2str(color.b, true) + " ";
				str += f2str(color.a, true);

				root.append_node("TextColor").append_attribute("value", str.c_str());
			}

			// TextAlign
			if (GetTextAlign() != eAlign::LEFT)
			{
				eAlign align = GetTextAlign();

				root.append_node("TextAlign").append_attribute("value", i2str(align._value).c_str());
			}
		}
		else
		{
			// Caption
			xml_node node = root.first_node("Caption");
			if (node != NULL) 
			{
				String caption = node.first_attribute("value");

				SetCaption(caption.c_wstr());
			}

			// Font
			node = root.first_node("Font");
			if (node != NULL)
			{
				FixedString32 fontName = node.first_attribute("value");

				SetFontName(fontName);
			}

			// TextColor
			node = root.first_node("TextColor");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");
				Float4 color = Float4(1, 1, 1, 1);
				str = str_getfloat(color.r, str);
				str = str_getfloat(color.g, str);
				str = str_getfloat(color.b, str);
				str = str_getfloat(color.a, str);

				SetTextColor(color);
			}

			// TextAlign
			node = root.first_node("TextAlign");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetTextAlign(atoi(str));
			}
		}
	}

}}