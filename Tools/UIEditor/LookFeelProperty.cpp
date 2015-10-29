#include "LookFeelProperty.h"
#include "AllLookFeel.h"
#include "LookFeelEditor.h"
#include "App.h"

LookFeelProperty::LookFeelProperty()
{
	mLayout = new MGUI::Layout(AllLookFeel::Instance()->GetPanel());

	float top = 0;
	const float K_Space = 8;

	// Name
	//
	MGUI::Label * lb_Name = new MGUI::Label(NULL, mLayout);
	lb_Name->SetCaption(L"Name:");
	lb_Name->SetRect(0, top , 64, 24);

	mEditBox_Name = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Name->SetCaption(L"");
	mEditBox_Name->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Skin
	//
	MGUI::Label * lb_Skin = new MGUI::Label(NULL, mLayout);
	lb_Skin->SetCaption(L"Skin:");
	lb_Skin->SetRect(0, top, 64, 24);

	mEditBox_Skin = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Skin->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// Client
	//
	MGUI::Label * lb_Client = new MGUI::Label(NULL, mLayout);
	lb_Client->SetCaption(L"Client:");
	lb_Client->SetRect(0, top, 64, 24);

	mEditBox_Client = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_Client->SetCaption(L"0 0 1 1");
	mEditBox_Client->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// State
	//
	MGUI::Label * lb_State = new MGUI::Label(NULL, mLayout);
	lb_State->SetCaption(L"State:");
	lb_State->SetRect(0, top, 64, 24);

	mComboBox_State = new MGUI::ComboBox(AllLookFeel::Instance()->GetComboBox(), mLayout);
	mComboBox_State->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// UV Rect
	//
	MGUI::Label * lb_UVRect = new MGUI::Label(NULL, mLayout);
	lb_UVRect->SetCaption(L"UVRect:");
	lb_UVRect->SetRect(0, top, 64, 24);

	mEditBox_UVRect = new MGUI::EditBox(AllLookFeel::Instance()->GetEditBox(), mLayout);
	mEditBox_UVRect->SetCaption(L"0 0 1 1");
	mEditBox_UVRect->SetRect(80, top, 162, 24);

	top += 24 + K_Space;

	// TextColor
	//
	MGUI::Label * lb_TextColor = new MGUI::Label(NULL, mLayout);
	lb_TextColor->SetCaption(WS("TextColor:"));
	lb_TextColor->SetRect(0, top, 64, 24);

	mWidget_TextColor = new MGUI::Widget(AllLookFeel::Instance()->GetWhite(), mLayout);
	mWidget_TextColor->SetRect(80, top, 162, 24);
	mWidget_TextColor->E_MouseClick += new cListener1<LookFeelProperty, const MGUI::MouseEvent *>(this, &LookFeelProperty::OnTextColorChanged);

	top += 24 + K_Space;

	// Children
	mListBox_Child = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout);
	mListBox_Child->SetRect(0, top, 128, 180);

	mButton_AddChild = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	mButton_AddChild->SetRect(133, top, 105, 24);
	mButton_AddChild->SetCaption(L"Add");
	mButton_AddChild->E_Click += new cListener1<LookFeelProperty, const MGUI::ClickEvent *>(this, &LookFeelProperty::OnAddChild);

	mButton_RemoveChild = new MGUI::Button(AllLookFeel::Instance()->GetButton(), mLayout);
	mButton_RemoveChild->SetRect(133, top + 24 + K_Space, 105, 24);
	mButton_RemoveChild->SetCaption(L"Remove");
	mButton_RemoveChild->E_Click += new cListener1<LookFeelProperty, const MGUI::ClickEvent *>(this, &LookFeelProperty::OnRemoveChild);

	mLookFeelSelectDlg = new LookFeelSelectDlg;
	mLookFeelSelectDlg->E_EndDialog += new cListener1<LookFeelProperty, bool>(this, &LookFeelProperty::OnLookFeelSelect_EndDialog);
	
	top += 180 + K_Space;

	// Init
	mEditBox_Skin->SetCaption(L"");

	for (int i = 0; i < MGUI::eWidgetState::MAX; ++i)
		mComboBox_State->Append(MGUI::eWidgetState::GetStateString(i).c_wstr());

	mComboBox_State->Append(L"All");

	mComboBox_State->SetSelectIndex(0);

	OnEditLookFeelChanged_(NULL);

	LookFeelEditor::Instance()->E_EditLookFeelChanged += new cListener1<LookFeelProperty, MGUI::LookFeel *>(this, &LookFeelProperty::OnEditLookFeelChanged_);
	LookFeelEditor::Instance()->E_EditRectChanged += new cListener1<LookFeelProperty, MGUI::Rect>(this, &LookFeelProperty::OnEditRectChanged_);

	mEditBox_Skin->E_KeyLostFocus += new cListener1<LookFeelProperty, const MGUI::FocusEvent *>(this, &LookFeelProperty::OnSkinChanged_);
	mComboBox_State->E_SelectChanged += new cListener2<LookFeelProperty, const MGUI::Event *, int>(this, &LookFeelProperty::OnStateChanged_);

	mEditBox_Name->E_KeyLostFocus += new cListener1<LookFeelProperty, const MGUI::FocusEvent *>(this, &LookFeelProperty::OnNameReturn_);
	mEditBox_Client->E_KeyLostFocus += new cListener1<LookFeelProperty, const MGUI::FocusEvent *>(this, &LookFeelProperty::OnClientReturn_);
	mEditBox_UVRect->E_KeyLostFocus += new cListener1<LookFeelProperty, const MGUI::FocusEvent *>(this, &LookFeelProperty::OnUVRectReturn_);
}

LookFeelProperty::~LookFeelProperty()
{
	delete mLookFeelSelectDlg;
	delete mLayout;
}

void LookFeelProperty::Layout()
{
	MGUI::Rect rect;

	rect.x = MGUI::Engine::Instance()->GetRect().w - 256;
	rect.y = 32;

	rect.w = 256;
	rect.h = MGUI::Engine::Instance()->GetRect().h - 32 - 32;

	mLayout->SetRect(rect);
}

void LookFeelProperty::OnEditLookFeelChanged_(MGUI::LookFeel * _lookfeel)
{
	if (_lookfeel)
	{
		int state = MGUI::eWidgetState::NORMAL;

		mEditBox_Name->SetCaption(_lookfeel->GetName().c_wstr());
		mEditBox_Client->SetCaption(_lookfeel->GetClientRect().ToString().c_wstr());

		mEditBox_UVRect->SetCaption(_lookfeel->GetUVRect(state).ToString().c_wstr());
		mWidget_TextColor->SetColor(_lookfeel->GetTextColor(state));

		if (_lookfeel->GetSkin() != "")
		{
			mEditBox_Skin->SetCaption(_lookfeel->GetSkin().c_wstr());
			mComboBox_State->SetSelectIndex(state);
		}
		else
		{
			mEditBox_Skin->SetCaption(L"");
		}

		mListBox_Child->Clear();

		for (int i = 0; i < _lookfeel->GetChildCount(); ++i)
		{
			MGUI::LookFeel * child = _lookfeel->GetChild(i);

			mListBox_Child->Append(child->GetName().c_wstr(), child);
		}
	}
	else
	{
		mEditBox_Name->SetCaption(WS(""));
		mEditBox_Client->SetCaption(MGUI::ClientRect().ToString().c_wstr());

		mEditBox_UVRect->SetCaption(MGUI::Rect().ToString().c_wstr());
		mWidget_TextColor->SetColor(Float4(1, 1, 1, 1));

		mEditBox_Skin->SetCaption(WS(""));

		mListBox_Child->Clear();
	}

	OnUVRectReturn_(NULL);
}

void LookFeelProperty::OnEditRectChanged_(MGUI::Rect _rect)
{
	mEditBox_UVRect->SetCaption(_rect.ToString().c_wstr());

	MGUI::LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();
	if (lookfeel == NULL)
		return ;

	int state = mComboBox_State->GetSelectIndex();
	if (state == -1)
		return ;

	if (state == MGUI::eWidgetState::MAX)
	{
		for (int i = 0; i < MGUI::eWidgetState::MAX; ++i)
		{
			lookfeel->SetUVRect(i, _rect);
		}
	}
	else
	{
		lookfeel->SetUVRect(state, _rect);
	}
}

void LookFeelProperty::OnSkinChanged_(const MGUI::FocusEvent * e)
{
	String skin;

	skin.FromUnicode(mEditBox_Skin->GetCaption().c_str());
	if (skin != "")
	{
		TexturePtr p = HWBufferManager::Instance()->LoadTexture(skin, -1);
			
		LookFeelEditor::Instance()->_notifySkinChanged(p.c_ptr());
	}
	else
	{
		LookFeelEditor::Instance()->_notifySkinChanged(NULL);
	}
}

void LookFeelProperty::OnStateChanged_(const MGUI::Event * e, int index)
{
	if (index == -1)
		return ;

	MGUI::LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

	if (lookfeel == NULL)
		return ;

	if (index == MGUI::eWidgetState::MAX)
		index = MGUI::eWidgetState::NORMAL;

	mEditBox_UVRect->SetCaption(lookfeel->GetUVRect(index).ToString().c_wstr());

	OnUVRectReturn_(NULL);
}

void LookFeelProperty::OnNameReturn_(const MGUI::FocusEvent * e)
{
	MGUI::LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

	if (lookfeel == NULL)
		return ;

	FixedString32 name;
	
	name.FromUnicode(mEditBox_Name->GetCaption().c_str());
	if (name == lookfeel->GetName())
		return ;

	if (name == "")
	{
		MessageBox(App::Instance()->_hWnd(), "look feel name can't be empty!", "Error", MB_ICONERROR | MB_OK);
	}
	else if (MGUI::LookFeelManager::Instance()->GetGroup(LookFeelEditor::Instance()->GetEditWidgetType())->Find(name) != NULL)
	{
		MessageBox(App::Instance()->_hWnd(), "look feel exist!", "Error", MB_ICONERROR | MB_OK);
	}
	else
	{
		lookfeel->SetName(name);
		LookFeelEditor::Instance()->_notifyNameChanged();
	}

	mEditBox_Name->SetCaption(lookfeel->GetName().c_wstr());
}

void LookFeelProperty::OnClientReturn_(const MGUI::FocusEvent * e)
{
	const UString & text = mEditBox_Client->GetCaption();
	String strRect;
	strRect.FromUnicode(text.c_str());

	MGUI::ClientRect rect;
	rect.FromString(strRect);

	LookFeelEditor::Instance()->_notifyClientChanged(rect);
}

void LookFeelProperty::OnUVRectReturn_(const MGUI::FocusEvent * e)
{
	const UString & text = mEditBox_UVRect->GetCaption();
	
	String strRect;
	strRect.FromUnicode(text.c_str());

	MGUI::Rect rect;
	rect.FromString(strRect);

	LookFeelEditor::Instance()->_notifyRectChanged(rect);
}

void LookFeelProperty::OnTextColorChanged(const MGUI::MouseEvent * e)
{
	MGUI::LookFeel * lookfeel = LookFeelEditor::Instance()->GetEditLookFeel();
	if (lookfeel == NULL)
		return ;

	int state = mComboBox_State->GetSelectIndex();
	if (state == -1)
		return ;

	Float4 color = mWidget_TextColor->GetColor();
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
		mWidget_TextColor->SetColor(color);

		if (state == MGUI::eWidgetState::MAX)
		{
			for (int i = 0; i < MGUI::eWidgetState::MAX; ++i)
			{
				lookfeel->SetTextColor(i, color);
			}
		}
		else
		{
			lookfeel->SetTextColor(state, color);
		}
	}
}

void LookFeelProperty::OnAddChild(const MGUI::ClickEvent *)
{
	mLookFeelSelectDlg->DoModal();
}

void LookFeelProperty::OnRemoveChild(const MGUI::ClickEvent *)
{
	int sel = mListBox_Child->GetSelectIndex();

	if (sel < 0)
		return ;

	MGUI::LookFeel * editLookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

	if (editLookfeel == NULL)
		return ;

	editLookfeel->RemoveChild(sel);
	
	mListBox_Child->Remove(sel);
}

void LookFeelProperty::OnLookFeelSelect_EndDialog(bool _ok)
{
	if (!_ok)
		return ;

	MGUI::LookFeel * lookfeel = mLookFeelSelectDlg->GetSelectLookFeel();

	if (lookfeel == NULL)
		return ;

	MGUI::LookFeel * editLookfeel = LookFeelEditor::Instance()->GetEditLookFeel();

	if (editLookfeel == NULL)
		return ;

	lookfeel = lookfeel->Clone(mLookFeelSelectDlg->GetNewName().c_str());
	editLookfeel->AddChild(lookfeel);
	
	mListBox_Child->Append(lookfeel->GetName().c_wstr(), lookfeel);
}
