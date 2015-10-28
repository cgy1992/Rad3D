#include "App.h"

const MGUI::LookFeel * gLookFeel_ListBox = NULL;
const MGUI::LookFeel * gLookFeel_EditBox = NULL;
const MGUI::LookFeel * gLookFeel_Button = NULL;

MGUI::EditBox * gEditBox = NULL;
MGUI::ListBox * gListBox = NULL;

#define SKIN_TEXTURE "Sample.png"

void InitLookFeel_ListBox()
{
	// ScrollBar
	MGUI::LookFeel * pBnVScrollUp = new MGUI::LookFeel("ScrollUp");
	pBnVScrollUp->SetSkin(SKIN_TEXTURE);
	pBnVScrollUp->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(95, 68, 14, 11));
	pBnVScrollUp->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(95, 54, 14, 11));
	pBnVScrollUp->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(95, 82, 14, 11));
	pBnVScrollUp->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(95, 96, 14, 11));
	pBnVScrollUp->SetClientRect(MGUI::ClientRect(0, 0, 0, 0));

	MGUI::LookFeel * pBnVScrollDown = new MGUI::LookFeel("ScrollDown");
	pBnVScrollDown->SetSkin(SKIN_TEXTURE);
	pBnVScrollDown->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(95, 15, 14, 11));
	pBnVScrollDown->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(95, 2, 14, 11));
	pBnVScrollDown->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(95, 28, 14, 11));
	pBnVScrollDown->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(95, 41, 14, 11));
	pBnVScrollDown->SetClientRect(MGUI::ClientRect(0, 0, 0, 0));

	MGUI::LookFeel * pBnVScroll = new MGUI::LookFeel("ScrollTrack");
	pBnVScroll->SetSkin(SKIN_TEXTURE);
	pBnVScroll->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(113, 57, 13, 18));
	pBnVScroll->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(113, 37, 13, 18));
	pBnVScroll->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(113, 77, 13, 18));
	pBnVScroll->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(113, 97, 13, 18));
	pBnVScroll->SetClientRect(MGUI::ClientRect(6, 6, 6, 6));

	MGUI::LookFeel * pVScrollBar = new MGUI::LookFeel("VScrollBar");
	pVScrollBar->SetSkin(SKIN_TEXTURE);
	pVScrollBar->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 73, 15,  19));
	pVScrollBar->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 73, 15,  19));
	pVScrollBar->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 73, 15,  19));
	pVScrollBar->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 73, 15,  19));
	pVScrollBar->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));
	pVScrollBar->AddChild(pBnVScroll);
	pVScrollBar->AddChild(pBnVScrollUp);
	pVScrollBar->AddChild(pBnVScrollDown);

	// ListBoxItem
	MGUI::LookFeel * pListBoxItem = new MGUI::LookFeel("ListBoxItem");
	pListBoxItem->SetSkin(SKIN_TEXTURE);
	pListBoxItem->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(204, 160, 42, 9));
	pListBoxItem->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(204, 160, 42, 9));
	pListBoxItem->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(204, 174, 42, 9));
	pListBoxItem->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(204, 188, 42, 9));
	pListBoxItem->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(204, 188, 42, 9));
	pListBoxItem->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));

	// ListBox
	MGUI::LookFeel * pListBox = new MGUI::LookFeel("ListBox");
	pListBox->SetSkin(SKIN_TEXTURE);
	pListBox->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 2, 22, 21));
	pListBox->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 2, 22, 21));
	pListBox->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 2, 22, 21));
	pListBox->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 2, 22, 21));
	pListBox->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(341, 2, 22, 21));
	pListBox->SetClientRect(MGUI::ClientRect(6, 6, 6, 6));
	pListBox->AddChild(pListBoxItem);
	pListBox->AddChild(pVScrollBar);

	gLookFeel_ListBox = pListBox;
}

void InitLookFeel_EidtBox()
{
	MGUI::LookFeel * pEditBox = new MGUI::LookFeel("EditBox");
	pEditBox->SetSkin(SKIN_TEXTURE);
	pEditBox->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(168, 28, 30, 27));
	pEditBox->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(168, 1, 30, 27));
	pEditBox->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(168, 55, 30, 27));
	pEditBox->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(168, 136, 30, 27));
	pEditBox->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(168, 136, 30, 27));
	pEditBox->SetClientRect(MGUI::ClientRect(5, 5, 5, 5));

	gLookFeel_EditBox = pEditBox;
}

void InitLookFeel_Button()
{
	MGUI::LookFeel * pButton = new MGUI::LookFeel("Button");
	pButton->SetSkin(SKIN_TEXTURE);
	pButton->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(112, 198, 18, 14));
	pButton->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(112, 183, 18, 14));
	pButton->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(112, 213, 18, 14));
	pButton->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(112, 228, 18, 14));
	pButton->SetClientRect(MGUI::ClientRect(6, 4, 6, 4));

	gLookFeel_Button = pButton;
}

void OnAdd(const MGUI::ClickEvent * e)
{
	const UString & caption = gEditBox->GetCaption();

	if (caption.Length() > 0)
	{
		gListBox->Append(caption);
	}
}

void OnRemove(const MGUI::ClickEvent * e)
{
	int selIndex = gListBox->GetSelectIndex();

	if (selIndex != -1)
	{
		gListBox->Remove(selIndex);
	}
}

void SetupUI()
{
	InitLookFeel_ListBox();
	InitLookFeel_EidtBox();
	InitLookFeel_Button();

	MGUI::FontManager::Instance()->Load("Sample.font");
	MGUI::LookFeelManager::Instance()->Load("Sample.lookfeel");

	MGUI::Layout * layout = MGUI::Layout::Load("Demo06/DemoGUI.layout", NULL);
	layout->SetAlign(MGUI::eAlign::CENTER);

	MGUI::TextBox * pTextBox_Caption = new MGUI::TextBox(NULL, layout->GetChild("Caption"));
	pTextBox_Caption->SetAlign(MGUI::eAlign::CENTER);
	pTextBox_Caption->SetCaption(WSTR_("DemoGUI"));

	// Create ListBox
	MGUI::ListBox * pListBox = new MGUI::ListBox(gLookFeel_ListBox, layout);
	pListBox->SetRect(20, 35, 180, 256);
	pListBox->Append(WSTR_("Item0"));
	pListBox->Append(WSTR_("Item1"));
	pListBox->Append(WSTR_("Item2"));
	gListBox = pListBox;

	// Create EditBox
	MGUI::EditBox * pEditBox = new MGUI::EditBox(gLookFeel_EditBox, layout);
	pEditBox->SetRect(220, 40, 180, 30);
	pEditBox->SetCaption(WSTR_("点击按钮添加或删除"));
	gEditBox = pEditBox;

	// Create Button
	MGUI::Button * pButtonAdd = new MGUI::Button(gLookFeel_Button, layout);
	pButtonAdd->SetRect(220, 80, 180, 30);
	pButtonAdd->SetCaption(WSTR_("Add"));
	pButtonAdd->E_Click += new ncListener1<const MGUI::ClickEvent *>(OnAdd);

	MGUI::Button * pButtonRemove = new MGUI::Button(gLookFeel_Button, layout);
	pButtonRemove->SetRect(220, 120, 180, 30);
	pButtonRemove->SetCaption(WSTR_("Remove"));
	pButtonRemove->E_Click += new ncListener1<const MGUI::ClickEvent *>(OnRemove);
}


class Demo06_GUI : public App
{
public:
	Demo06_GUI() {}
	virtual ~Demo06_GUI() {}

	virtual void OnSetupResource()
	{
#ifdef M_PLATFORM_WIN32
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Core"));
		ResourceManager::Instance()->AddArchive(new FilePathArchive("../Sample"));
#endif

#ifdef M_PLATFORM_ANDROID
		ResourceManager::Instance()->AddArchive(new APKArchive("Core"));
		ResourceManager::Instance()->AddArchive(new APKArchive("Sample"));
#endif
	}

	virtual void OnInit()
	{
		CharSet::SetCharSet("gb2312");

		SetupUI();
	}

	virtual void OnUpdate()
	{
	}

	virtual void OnShutdown()
	{
		delete gLookFeel_ListBox;
		delete gLookFeel_EditBox;
		delete gLookFeel_Button;
	}

	virtual void OnPause()
	{
	}

	virtual void OnResume()
	{
	}
};

void CreateApplication(App ** ppApp)
{
	*ppApp = new Demo06_GUI;
}
