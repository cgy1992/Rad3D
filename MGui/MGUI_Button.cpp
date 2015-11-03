#include "MGUI_Button.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(Button, Widget);

	Button::Button(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mTextBox = new TextBox(_lookfeel, this);
		mTextBox->SetAlign(eAlign::CENTER);
		mTextBox->SetPickFlag(ePickFlag::NONE);
		mTextBox->SetInheritState(true);

		mTranslation = false;
	}

	Button::~Button()
	{
	}

	void Button::SetCaption(const UString & caption)
	{
		mTextBox->SetCaption(caption);
	}

	const UString & Button::GetCaption() const
	{
		return mTextBox->GetCaption();
	}

	void Button::SetFontName(const FixedString32 & name)
	{
		Font * p = FontManager::Instance()->GetFont(name);
		if (p)
		{
			mTextBox->SetFont(p);
		}
	}

	FixedString32 Button::GetFontName() const
	{
		Font * font = mTextBox->GetFont();
		if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
		{
			return font->GetName();
		}

		return "";
	}

	void Button::SetTextColor(const Float4 & color)
	{
		mTextBox->SetColor(color);
	}

	const Float4 & Button::GetTextColor() const
	{
		return mTextBox->GetColor();
	}

	void Button::SerializeXML(xml_node root, bool is_out)
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

			Font * font = _getFont();
			if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
			{
				root.append_node("Font").append_attribute("value", GetFontName().c_str());
			}
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

}}