#include "MGUI_Label.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(Label, Widget);

	Label::Label(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mTextBox = new TextBox(_lookfeel, this);
		mTextBox->SetPickFlag(ePickFlag::NONE);
		mTextBox->SetInheritState(true);

		SetTextAlign(eAlign::LEFT);

		mTranslation = false;
	}

	Label::~Label()
	{
	}

	void Label::SetCaption(const UString & caption)
	{
		mTextBox->SetCaption(caption);
	}

	const UString & Label::GetCaption() const
	{
		return mTextBox->GetCaption();
	}

	void Label::SetFontName(const FixedString32 & name)
	{
		Font * p = FontManager::Instance()->GetFont(name);
		if (p)
		{
			mTextBox->SetFont(p);
		}
	}

	FixedString32 Label::GetFontName() const
	{
		Font * font = mTextBox->GetFont();
		if (font != NULL && font != FontManager::Instance()->GetDefaultFont())
		{
			return font->GetName();
		}

		return "";
	}

	void Label::SetTextColor(const Float4 & color)
	{
		mTextBox->SetColor(color);
	}

	const Float4 & Label::GetTextColor() const
	{
		return mTextBox->GetColor();
	}

	void Label::SetTextAlign(eAlign _align)
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

	eAlign Label::GetTextAlign() const
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

	void Label::AdjustBestSize()
	{
		Rect rc = mTextBox->GetGlyphRect();

		if (mLookFeel != NULL)
		{
			const ClientRect & client = mLookFeel->GetClientRect();

			rc.w += client.dx0 + client.dx1;
			rc.h += client.dy0 + client.dy1;
		}

		rc.x = mRect.x;
		rc.y = mRect.y;

		SetRect(rc);
	}

	void Label::SerializeXML(xml_node root, bool is_out)
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

			if (GetTextColor() != Float4(1, 1, 1, 1))
			{
				const Float4 & color = GetTextColor();
				String str;
				str += f2str(color.r, true) + " ";
				str += f2str(color.g, true) + " ";
				str += f2str(color.b, true) + " ";
				str += f2str(color.a, true);

				root.append_node("TextColor").append_attribute("TextColor", str.c_str());
			}

			if (GetTextAlign() != eAlign::LEFT)
			{
				eAlign align = GetTextAlign();

				root.append_node("TextAlign").append_attribute("value", i2str(align._value).c_str());
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

			node = root.first_node("TextColor");
			if (node != NULL)
			{
				Float4 color = Float4(1, 1, 1, 1);
				const char * str = node.first_attribute("value");
				str = str_getfloat(color.r, str);
				str = str_getfloat(color.g, str);
				str = str_getfloat(color.b, str);
				str = str_getfloat(color.a, str);

				SetTextColor(color);
			}

			node = root.first_node("TextAlign");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetTextAlign(atoi(str));
			}
		}
	}

}}