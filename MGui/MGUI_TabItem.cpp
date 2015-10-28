#include "MGUI_TabItem.h"
#include "MGUI_TabControl.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(TabItem, Widget);

	TabItem::TabItem(const LookFeel * lookfeel, TabControl * parent, const UString & caption, void * usedata)
		: Widget(lookfeel, parent)
		, mTabControl(parent)
	{
		mTextBox = new TextBox(lookfeel, this);
		mTextBox->SetPickFlag(ePickFlag::NONE);
		mTextBox->SetInheritState(true);
		mTextBox->SetCaption(caption);

		SetUserData(usedata);
	}

	TabItem::~TabItem()
	{
	}

	void TabItem::SetText(const UString & caption)
	{
		mTextBox->SetCaption(caption);
	}

	const UString & TabItem::GetText()
	{
		return mTextBox->GetCaption();
	}

	void TabItem::SetFont(Font * font)
	{
		mTextBox->SetFont(font);
	}

	Font * TabItem::GetFont()
	{
		return mTextBox->GetFont();
	}

	void TabItem::_updateRect()
	{
		Rect rectState = mLookFeel->GetUVRect(mState);
		ClientRect clRect = mLookFeel->GetClientRect();
		Rect rectGlyph = mTextBox->GetGlyphRect();

		Rect myRect;

		myRect.x = 0;
		myRect.y = 0;
		myRect.w = rectState.w;
		myRect.h = rectState.h;

		if (rectGlyph.w > rectState.w)
		{
			myRect.w = rectGlyph.w + clRect.dx0 + clRect.dx1;
		}
	}

	void TabItem::OnUpdateWidgetState()
	{
		Widget::OnUpdateWidgetState();

		if (mTabControl->IsEnable())
		{
			if (mTabControl->GetSelectItem() == this)
			{
				mState = eWidgetState::SELECT;
			}
		}
		else
		{
			mState = eWidgetState::DISABLE;
		}
	}

}}

