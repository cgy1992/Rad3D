#include "LayoutProperty.h"
#include "LayoutEditor.h"
#include "AllLookFeel.h"
#include "App.h"

#include "TypeEd_Button.h"
#include "TypeEd_CheckBox.h"
#include "TypeEd_EditBox.h"
#include "TypeEd_EditView.h"
#include "TypeEd_ImageBox.h"
#include "TypeEd_Label.h"
#include "TypeEd_ListBox.h"
#include "TypeEd_ComboBox.h"

LayoutProperty::LayoutProperty()
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel());

	float top = 0;
	const float K_Space = 8;

	// Name
	MGUI::Label * lb_Name = new MGUI::Label(NULL, mLayout);
	lb_Name->SetCaption(L"Name:");
	lb_Name->SetRect(0, top , 64, 24);

	mEditBox_Name = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Name->SetCaption(L"");
	mEditBox_Name->SetRect(80, top, 162, 24);
	top += 24 + K_Space;

	// Rect
	MGUI::Label * lb_Rect = new MGUI::Label(NULL, mLayout);
	lb_Rect->SetCaption(L"Rect:");
	lb_Rect->SetRect(0, top, 64, 24);

	mEditBox_Rect = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Rect->SetCaption(L"0 0 1 1");
	mEditBox_Rect->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Order
	MGUI::Label * lb_Order = new MGUI::Label(NULL, mLayout);
	lb_Order->SetCaption(L"Order:");
	lb_Order->SetRect(0, top, 64, 24);

	mEditBox_Order = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Order->SetCaption(L"0 0 1 1");
	mEditBox_Order->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Color
	MGUI::Label * lb_TextColor = new MGUI::Label(NULL, mLayout);
	lb_TextColor->SetCaption(L"Color:");
	lb_TextColor->SetRect(0, top, 64, 24);

	mWidget_Color = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mLayout);
	mWidget_Color->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Opacity
	MGUI::Label * lb_Opacity = new MGUI::Label(NULL, mLayout);
	lb_Opacity->SetCaption(L"Opacity:");
	lb_Opacity->SetRect(0, top, 64, 24);

	mEditBox_Opacity = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Opacity->SetCaption(L"1");
	mEditBox_Opacity->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// InheritAlpha
	MGUI::Label * lb_InheritAlpha = new MGUI::Label(NULL, mLayout);
	lb_InheritAlpha->SetCaption(L"InheritAlpha:");
	lb_InheritAlpha->SetRect(0, top, 64, 24);

	mComboBox_InheritAlpha = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_InheritAlpha->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// H Align
	MGUI::Label * lb_HAlign = new MGUI::Label(NULL, mLayout);
	lb_HAlign->SetCaption(L"HAlign:");
	lb_HAlign->SetRect(0, top, 64, 24);

	mComboBox_HAlign = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_HAlign->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// V Align
	MGUI::Label * lb_VAlign = new MGUI::Label(NULL, mLayout);
	lb_VAlign->SetCaption(L"VAlign:");
	lb_VAlign->SetRect(0, top, 64, 24);

	mComboBox_VAlign = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_VAlign->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Enable
	MGUI::Label * lb_Enable = new MGUI::Label(NULL, mLayout);
	lb_Enable->SetCaption(L"Enable:");
	lb_Enable->SetRect(0, top, 64, 24);

	mComboBox_Enable = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_Enable->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Pick Enable
	MGUI::Label * lb_PickEnable = new MGUI::Label(NULL, mLayout);
	lb_PickEnable->SetCaption(L"Pick:");
	lb_PickEnable->SetRect(0, top, 64, 24);

	mComboBox_PickFlag = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_PickFlag->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Type Property
	mTypeEdPanel = new MGUI::Panel(NULL, mLayout);
	mTypeEdPanel->SetRect(0, top, 256, 384);
	mTypeEdPanel->SetAlign(MGUI::eAlign::H_STRETCH);
	mTypeEdPanel->SetVScrollBar(AllLookFeel::Instance()->GetVScrollBar());

	top += 384 + K_Space;

	// Init
	mComboBox_InheritAlpha->Append(L"False");
	mComboBox_InheritAlpha->Append(L"True");

	mComboBox_HAlign->Append(L"None");
	mComboBox_HAlign->Append(L"Left");
	mComboBox_HAlign->Append(L"Center");
	mComboBox_HAlign->Append(L"Right");
	mComboBox_HAlign->Append(L"Stretch");

	mComboBox_VAlign->Append(L"None");
	mComboBox_VAlign->Append(L"Top");
	mComboBox_VAlign->Append(L"Center");
	mComboBox_VAlign->Append(L"Bottom");
	mComboBox_VAlign->Append(L"Stretch");

	mComboBox_Enable->Append(L"False");
	mComboBox_Enable->Append(L"True");

	mComboBox_PickFlag->Append(L"All");
	mComboBox_PickFlag->Append(L"Self");
	mComboBox_PickFlag->Append(L"Child");
	mComboBox_PickFlag->Append(L"None");

	mComboBox_HAlign->SetSelectIndex(0);
	mComboBox_VAlign->SetSelectIndex(0);
	mComboBox_Enable->SetSelectIndex(0);
	mComboBox_PickFlag->SetSelectIndex(0);

	// register E_
	mEditBox_Name->E_KeyLostFocus += new cListener1<LayoutProperty, const MGUI::FocusEvent *>(this, &LayoutProperty::OnNameChanged_);
	mEditBox_Rect->E_KeyLostFocus += new cListener1<LayoutProperty, const MGUI::FocusEvent *>(this, &LayoutProperty::OnRectChanged_);
	mEditBox_Order->E_KeyLostFocus += new cListener1<LayoutProperty, const MGUI::FocusEvent *>(this, &LayoutProperty::OnOrderChanged_);
	mWidget_Color->E_MouseClick += new cListener1<LayoutProperty, const MGUI::MouseEvent *>(this, &LayoutProperty::OnColorChanged_);
	mEditBox_Opacity->E_KeyLostFocus += new cListener1<LayoutProperty, const MGUI::FocusEvent *>(this, &LayoutProperty::OnOpacityChanged_);
	mComboBox_InheritAlpha->E_SelectChanged += new cListener2<LayoutProperty, const MGUI::Event *, int>(this, &LayoutProperty::OnInheritAlphaChanged_);
	mComboBox_HAlign->E_SelectChanged += new cListener2<LayoutProperty, const MGUI::Event *, int>(this, &LayoutProperty::OnAlignChanged_);
	mComboBox_VAlign->E_SelectChanged += new cListener2<LayoutProperty, const MGUI::Event *, int>(this, &LayoutProperty::OnAlignChanged_);
	mComboBox_Enable->E_SelectChanged += new cListener2<LayoutProperty, const MGUI::Event *, int>(this, &LayoutProperty::OnEnableChanged_);
	mComboBox_PickFlag->E_SelectChanged += new cListener2<LayoutProperty, const MGUI::Event *, int>(this, &LayoutProperty::OnPickFlagChanged_);

	// 
	mTypeEd = NULL;

	LayoutEditor::Instance()->E_EditWidgetChanged += new cListener1<LayoutProperty, MGUI::Widget *>(this, &LayoutProperty::OnEditWidgetChanged_);
	LayoutEditor::Instance()->E_RectChanged += new cListener1<LayoutProperty, MGUI::Rect>(this, &LayoutProperty::OnEditRectChanged_);
}

LayoutProperty::~LayoutProperty()
{
	safe_delete (mTypeEd);
	delete mLayout;
}

void LayoutProperty::Layout()
{
	MGUI::Rect rect;

	rect.x = MGUI::Engine::Instance()->GetRect().w - 256;
	rect.y = 32;
	rect.w = 256;
	rect.h = MGUI::Engine::Instance()->GetRect().h - 32 - 32;

	mLayout->SetRect(rect);
}

void LayoutProperty::OnNameChanged_(const MGUI::FocusEvent * e)
{
	String name;
	name.FromUnicode(mEditBox_Name->GetCaption().c_str());

	LayoutEditor::Instance()->notifyNameChanged(name);
}

void LayoutProperty::OnRectChanged_(const MGUI::FocusEvent * e)
{
	const UString & text = mEditBox_Rect->GetCaption();

	String strRect;
	strRect.FromUnicode(text.c_str());

	MGUI::Rect rect;
	rect.FromString(strRect);

	LayoutEditor::Instance()->notifyRectChanged(rect);
}

void LayoutProperty::OnOrderChanged_(const MGUI::FocusEvent * e)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();

	if (widget == NULL)
		return ;

	const UString & text = mEditBox_Order->GetCaption();
	
	String str;
	str.FromUnicode(text.c_str());

	int order = str.ToInt();

	widget->SetOrder(order);
}

void LayoutProperty::OnColorChanged_(const MGUI::MouseEvent * e)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();

	if (widget == NULL)
		return ;

	Float4 color = widget->GetColor();
	color.a = 1;

	static COLORREF crCustColors[6];
	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.hwndOwner = App::Instance()->_hWnd();
	cc.hInstance = NULL;
	cc.rgbResult = RGB(color.r * 255, color.g * 255, color.b * 255);
	cc.lpCustColors = crCustColors;
	cc.Flags = CC_RGBINIT|CC_FULLOPEN;
	cc.lCustData = 0;
	cc.lpfnHook = NULL;
	cc.lpTemplateName = NULL;

	if (ChooseColor(&cc))
	{
		color.r = GetRValue(cc.rgbResult) / 255.0f;
		color.g = GetGValue(cc.rgbResult) / 255.0f;
		color.b = GetBValue(cc.rgbResult) / 255.0f;
		widget->SetColor(color);
		mWidget_Color->SetColor(color);
	}
}

void LayoutProperty::OnOpacityChanged_(const MGUI::FocusEvent * e)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();

	if (widget == NULL)
		return ;

	const UString & text = mEditBox_Opacity->GetCaption();

	String str;
	str.FromUnicode(text.c_str());

	widget->SetAlpha(str.ToFloat());
}

void LayoutProperty::OnInheritAlphaChanged_(const MGUI::Event * e, int _index)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();

	if (widget == NULL)
		return ;

	widget->SetInheritAlpha(_index == 1);
}

void LayoutProperty::OnAlignChanged_(const MGUI::Event * e, int _index)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();
	if (widget == NULL)
		return ;

	int hIndex = mComboBox_HAlign->GetSelectIndex();
	int vIndex = mComboBox_VAlign->GetSelectIndex();

	if (hIndex == -1 || vIndex == -1)
		return ;

	const int hAlign[] = {
		MGUI::eAlign::NONE,
		MGUI::eAlign::LEFT,
		MGUI::eAlign::H_CENTER,
		MGUI::eAlign::RIGHT,
		MGUI::eAlign::H_STRETCH
	};

	const int vAlign[] = {
		MGUI::eAlign::NONE,
		MGUI::eAlign::TOP,
		MGUI::eAlign::V_CENTER,
		MGUI::eAlign::BOTTOM,
		MGUI::eAlign::V_STRETCH
	};

	int flag = hAlign[hIndex] | vAlign[vIndex];

	widget->SetAlign(flag);
}

void LayoutProperty::OnEnableChanged_(const MGUI::Event * e, int _index)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();
	if (widget == NULL)
		return ;

	bool enable = (_index == 1);
	widget->SetEnable(enable);
}

void LayoutProperty::OnPickFlagChanged_(const MGUI::Event * e, int _index)
{
	MGUI::Widget * widget = LayoutEditor::Instance()->GetEditWidget();

	if (widget == NULL)
		return ;

	int flag[] = {
		MGUI::ePickFlag::ALL,
		MGUI::ePickFlag::SELF,
		MGUI::ePickFlag::CHILD,
		MGUI::ePickFlag::NONE
	};

	widget->SetPickFlag(flag[_index]);
}

void LayoutProperty::OnEditWidgetChanged_(MGUI::Widget * _widget)
{
	safe_delete (mTypeEd);

	if (_widget != NULL)
	{
		mEditBox_Name->SetCaption(_widget->GetName().c_wstr());
		mEditBox_Rect->SetCaption(_widget->GetRect().ToString().c_wstr());

		{
			String str;
			str.Format("%d", _widget->GetOrder());
			mEditBox_Order->SetCaption(str.c_wstr());
		}

		mWidget_Color->SetColor(_widget->GetColor());
		mWidget_Color->SetAlpha(1);
		mEditBox_Opacity->SetCaption(f2str(_widget->GetColor().a, true).c_wstr());
		mComboBox_InheritAlpha->SetSelectIndex(_widget->IsInheritAlpha() ? 1 : 0);

		int align = _widget->GetAlign()._value;

		if (align & MGUI::eAlign::LEFT)
			mComboBox_HAlign->SetSelectIndex(1);
		else if (align & MGUI::eAlign::H_CENTER)
			mComboBox_HAlign->SetSelectIndex(2);
		else if (align & MGUI::eAlign::RIGHT)
			mComboBox_HAlign->SetSelectIndex(3);
		else if (align & MGUI::eAlign::H_STRETCH)
			mComboBox_HAlign->SetSelectIndex(4);
		else
			mComboBox_HAlign->SetSelectIndex(0);

		if (align & MGUI::eAlign::TOP)
			mComboBox_VAlign->SetSelectIndex(1);
		else if (align & MGUI::eAlign::V_CENTER)
			mComboBox_VAlign->SetSelectIndex(2);
		else if (align & MGUI::eAlign::BOTTOM)
			mComboBox_VAlign->SetSelectIndex(3);
		else if (align & MGUI::eAlign::V_STRETCH)
			mComboBox_VAlign->SetSelectIndex(4);
		else
			mComboBox_VAlign->SetSelectIndex(0);

		mComboBox_Enable->SetSelectIndex(_widget->IsEnable() ? 1 : 0);

		int pk = _widget->GetPickFlag();

		if (pk == MGUI::ePickFlag::ALL)
			mComboBox_PickFlag->SetSelectIndex(0);
		else if (pk == MGUI::ePickFlag::SELF)
			mComboBox_PickFlag->SetSelectIndex(1);
		else if (pk == MGUI::ePickFlag::CHILD)
			mComboBox_PickFlag->SetSelectIndex(2);
		else
			mComboBox_PickFlag->SetSelectIndex(3);

		if (TYPE_OF(MGUI::Button, _widget))
		{
			mTypeEd = new TypeEd_Button(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::CheckBox, _widget))
		{
			mTypeEd = new TypeEd_CheckBox(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::EditBox, _widget))
		{
			mTypeEd = new TypeEd_EditBox(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::EditView, _widget))
		{
			mTypeEd = new TypeEd_EditView(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::ImageBox, _widget))
		{
			mTypeEd = new TypeEd_ImageBox(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::Label, _widget))
		{
			mTypeEd = new TypeEd_Label(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::ListBox, _widget))
		{
			mTypeEd = new TypeEd_ListBox(_widget, mTypeEdPanel);
		}
		else if (TYPE_OF(MGUI::ComboBox, _widget))
		{
			mTypeEd = new TypeEd_ComboBox(_widget, mTypeEdPanel);
		}
	}
	else
	{
	}
}

void LayoutProperty::OnEditRectChanged_(MGUI::Rect rect)
{
	mEditBox_Rect->SetCaption(rect.ToString().c_wstr());
}
