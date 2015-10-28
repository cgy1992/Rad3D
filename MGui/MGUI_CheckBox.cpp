#include "MGUI_CheckBox.h"
#include "MGUI_Engine.h"

namespace Rad { namespace MGUI {

	ImplementRTTI(CheckBox, Widget);

	CheckBox::CheckBox(const LookFeel * _lookfeel, Widget * _parent)
		: Widget(_lookfeel, _parent)
	{
		mChecked = false;
	}

	CheckBox::~CheckBox()
	{
	}

	void CheckBox::OnUpdateWidgetState()
	{
		Widget::OnUpdateWidgetState();

		if (mChecked)
		{
			mState += eWidgetState::SELECT;
		}
	}

	void CheckBox::OnMouseUp(int _id, float _x, float _y)
	{
		if (_id == InputCode::MKC_LEFT)
		{
			mChecked = !mChecked;

			Event e(&E_Checked);
			e.sender = this;

			E_Checked(&e, mChecked);
		}
	}

	void CheckBox::OnTouchUp(int _id, float _x, float _y)
	{
		mChecked = !mChecked;

		Event e(&E_Checked);
		e.sender = this;

		E_Checked(&e, mChecked);
	}

	void CheckBox::SerializeXML(xml_node root, bool is_out)
	{
		Widget::SerializeXML(root, is_out);

		if (is_out)
		{
			if (mChecked)
			{
				root.append_node("Checked").append_attribute("value", "1");
			}
		}
		else
		{
			xml_node node = root.first_node("Checked");
			if (node != NULL)
			{
				const char * str = node.first_attribute("value");

				SetChecked(atoi(str) == 0);
			}
		}
	}
}}