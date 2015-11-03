#include "MGUI_EditView.h"
#include "MGUI_Engine.h"
#include "MGUI_Helper.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(EditView, Panel);

	EditView::EditView(const LookFeel * _lookfeel, Widget * _parent)
		: Panel(_lookfeel, _parent)
	{
		mTextView = new TextView(_lookfeel, mClientWidget);
		mTextView->SetAlign(eAlign::STRETCH);
		mTextView->SetAutoLineFeed(true);
		mTextView->SetPickFlag(ePickFlag::NONE);
		mTextView->SetInheritState(true);

		mTranslation = false;

		mSelectBackColor = Float4(0.4f, 0.4f, 0.75f);
		mSelectCursorColor = Float4(1, 1, 1);

		mSelectIndex = mSelectStartIndex = mSelectEndIndex = 0;
		mDragIndex = 0;

		mStatic = false;

		mSelectVisible = false;
		mTimer = new Timer;
		mTimer->SetTime(0.5f, 0);
		mTimer->SetEnable(false);
		mTimer->E_Timer += new cListener1<EditView, Component *>(this, &EditView::OnTimer);
		AddComponent(mTimer);
	}

	EditView::~EditView()
	{
	}

	void EditView::SetCaption(const UString & caption, bool _sendEvent)
	{
		mTextView->SetCaption(caption);

		mSelectIndex = Min(caption.Length(), mSelectIndex);
		mSelectStartIndex = mSelectEndIndex = mSelectIndex;

		if (_sendEvent)
		{
			Event e(&E_TextChanged);
			e.sender = this;
			E_TextChanged(&e, caption);
		}
	}

	const UString & EditView::GetCaption() const
	{
		return mTextView->GetCaption();
	}

	void EditView::SetFontName(const FixedString32 & name)
	{
		Font * p = FontManager::Instance()->GetFont(name);
		if (p)
		{
			mTextView->SetFont(p);
		}
	}

	FixedString32 EditView::GetFontName() const
	{
		Font * font = mTextView->GetFont();
		if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
		{
			return font->GetName();
		}

		return "";
	}

	void EditView::SetTextColor(const Float4 & color)
	{
		mTextView->SetColor(color);
	}

	const Float4 & EditView::GetTextColor() const
	{
		return mTextView->GetColor();
	}

	void EditView::SetStatic(bool _static)
	{
		mStatic = _static;

		if (InputManager::Instance()->GetKeyFocusedWidget() == this)
			InputManager::Instance()->SetKeyFocusedWidget(NULL);
	}

	void EditView::OnUpdate(float elapsedTime)
	{
		Panel::OnUpdate(elapsedTime);

		mTextView->SetMaxWidth(GetAbsClient().w);
	}

	void EditView::UpdateRenderItem(Layout * _layout)
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

		if (mSelectEndIndex > mSelectStartIndex && mState == eWidgetState::SELECT)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder() + eOrder::TEXT_BACK, Helper::Instance()->GetWhiteTexture().c_ptr());

			mTextView->_updateSelectedRenderItem(ri, mSelectBackColor, mSelectStartIndex, mSelectEndIndex);
		}

		for (int i = 0; i < mChildren.Size(); ++i)
		{
			Widget * child = GetChild(i);
			child->UpdateRenderItem(_layout);
		}

		if (mTextView->GetFont() == NULL)
			return ;

		if (mSelectVisible)
		{
			RenderItem * ri = _layout->GetRenderItem(GetAbsOrder() + eOrder::TEXT_CURSOR, Helper::Instance()->GetWhiteTexture().c_ptr());

			mTextView->_updateSelectorRenderItem(ri, mSelectCursorColor, mSelectIndex);
		}
	}

	void EditView::SerializeXML(xml_node root, bool is_out)
	{
		Widget::SerializeXML(root, is_out);

		if (is_out)
		{
			if (mTranslation)
			{
				root.append_node("Translation").append_attribute("value", "true");
			}

			if (GetCaption().Length() > 0)
			{
				String caption;

				caption.FromUnicode(GetCaption().c_str());

				root.append_node("Caption").append_attribute("value", caption.c_str());
			}

			root.append_node("Caption").append_attribute("value", GetFontName().c_str());
		}
		else
		{
			xml_node node = root.first_node("Translation");
			if (node != NULL)
			{
				mTranslation = strcmp(node.first_attribute("value"), "true") == 0;
			}

			node = root.first_node("Caption");
			if (node != NULL) 
			{
				String caption = node.first_attribute("value");

				SetCaption(L_TR(caption, mTranslation).c_wstr());
			}

			node = root.first_node("Font");
			if (node != NULL) 
			{
				FixedString32 fontName = node.first_attribute("value");

				SetFontName(fontName);
			}
		}
	}

	void EditView::OnMouseDown(int _id, float _x, float _y)
	{
		if (mStatic)
			return ;

		int selectIndex = mTextView->_mapTextIndex(_x, _y);

		mSelectIndex = mSelectStartIndex = mSelectEndIndex = Max(selectIndex, 0);

		mDragIndex = mSelectIndex;

#ifdef M_PLATFORM_ANDROID
		InputManager::Instance()->E_OpenIMEKeyboard();
#endif
	}

	void EditView::OnMouseDrag(float _x, float _y)
	{
		if (mStatic)
			return ;

		int selectIndex = mTextView->_mapTextIndex(_x, _y);

		if (selectIndex < mDragIndex)
		{
			mSelectStartIndex = selectIndex;
			mSelectEndIndex = mDragIndex;
		}
		else
		{
			mSelectStartIndex = mDragIndex;
			mSelectEndIndex = selectIndex;
		}

		mSelectIndex = selectIndex;
	}

	void EditView::OnTouchDown(int _id, float _x, float _y)
	{
		OnMouseDown(InputCode::MKC_LEFT, _x, _y);
	}

	void EditView::OnKeyLostFocus(Widget * _new)
	{
		mSelectVisible = false;
		mTimer->SetEnable(false);
	}

	void EditView::OnKeySetFocus(Widget* _old)
	{
		if (mEnable && !mStatic)
		{
			mSelectVisible = true;
			mTimer->SetEnable(true);

			InputManager::Instance()->SetEditString(GetCaption());
		}
	}

	void EditView::OnKeyDown(int _key, uchar_t _char)
	{
		if (!mEnable || mStatic)
			return ;

		UString caption = mTextView->GetCaption();

		if (_key == InputCode::KC_ESCAPE)
			InputManager::Instance()->SetKeyFocusedWidget(NULL);

		else if (
			_key == InputCode::KC_BACK ||
			_key == InputCode::KC_DELETE)
		{
			if (mSelectEndIndex == 0)
				return ;

			if (mSelectEndIndex > mSelectStartIndex)
			{
				caption.Erase(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				mTextView->SetCaption(caption);
			}
			else
			{
				caption.Erase(mSelectIndex - 1, 1);
				mSelectIndex = mSelectIndex - 1;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;

				mTextView->SetCaption(caption);
			}

			mDragIndex = mSelectIndex;
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

			mDragIndex = mSelectIndex;

			if (!mStatic)
			{
				mSelectVisible = true;
				mTimer->Reset();
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

			mDragIndex = mSelectIndex;

			if (!mStatic)
			{
				mSelectVisible = true;
				mTimer->Reset();
			}
		}

		else if (_key == InputCode::KC_UPARROW)
		{
			int line = 0, offset = 0;
			mTextView->_getLineOffset(line, offset, mSelectIndex);

			mSelectIndex = mTextView->_getPrevLineIndex(line, offset, mSelectIndex);
			mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			mDragIndex = mSelectIndex;

			if (!mStatic)
			{
				mSelectVisible = true;
				mTimer->Reset();
			}
		}
		
		else if (_key == InputCode::KC_DOWNARROW)
		{
			int line = 0, offset = 0;
			mTextView->_getLineOffset(line, offset, mSelectIndex);

			mSelectIndex = mTextView->_getNextLineIndex(line, offset, mSelectIndex);
			mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			mDragIndex = mSelectIndex;

			if (!mStatic)
			{
				mSelectVisible = true;
				mTimer->Reset();
			}
		}

		else if (
			_key == InputCode::KC_NUMPADENTER ||
			_key == InputCode::KC_ENTER)
		{
			if (mSelectEndIndex > mSelectStartIndex)
			{
				caption.Erase(mSelectStartIndex, mSelectEndIndex - mSelectStartIndex);

				mSelectIndex = mSelectStartIndex;
				mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			}

			caption.Insert(mSelectIndex, WCHAR_('\n'));

			mSelectIndex += 1;
			mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			mDragIndex = mSelectIndex;

			mTextView->SetCaption(caption);
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

				mDragIndex = mSelectIndex;

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

					mSelectIndex += text.Length();
					mSelectStartIndex = mSelectEndIndex = mSelectIndex;
					mDragIndex = mSelectIndex;

					mTextView->SetCaption(caption);
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
					mDragIndex = mSelectIndex;

					mTextView->SetCaption(caption);
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

			mSelectIndex += 1;
			mSelectStartIndex = mSelectEndIndex = mSelectIndex;
			mDragIndex = mSelectIndex;

			mTextView->SetCaption(caption);

			if (!mStatic)
			{
				mSelectVisible = true;
				mTimer->Reset();
			}
		}
	}

	void EditView::OnTimer(Component *)
	{
		mSelectVisible = !mSelectVisible;
	}

}}