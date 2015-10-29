#include "Console.h"
#include "AllLookFeel.h"
#include "RadC.h"

void console_error_reporter(const char * err)
{
	Console::Instance()->Print(err);
};

ImplementSingleton(Console);

Console::Console()
{
	mLayout = MGUI::Layout::Load("RadCConsole.layout", NULL);
	mLayout->SetOrder(MGUI::eOrder::TOP - 100);
	mLayout->SetAlign(MGUI::eAlign::CENTER);
	mLayout->SetVisible(false);

	mListBox = new MGUI::ListBox(AllLookFeel::Instance()->GetListBox(), mLayout->GetChild("ErrorList"));
	mListBox->SetAlign(MGUI::eAlign::STRETCH);

	MGUI::Button * bnClose = mLayout->GetChildT<MGUI::Button>("Close");
	bnClose->E_Click += new cListener1<Console, const MGUI::ClickEvent *>(this, &Console::OnClose);

	MGUI::Button * bnClear = mLayout->GetChildT<MGUI::Button>("Clear");
	bnClear->E_Click += new cListener1<Console, const MGUI::ClickEvent *>(this, &Console::OnClear);

	radc_setErrorReporter(console_error_reporter);
}

Console::~Console()
{
	delete mLayout;
}

void Console::SetVisible(bool b)
{
	mLayout->SetVisible(b);
}

bool Console::IsVisible()
{
	return mLayout->IsVisible();
}

void Console::Print(const char * fmt, ...)
{
	FixedString256 text;
	va_list arglist;

	va_start(arglist, fmt);
	vsprintf(text.c_str(), fmt, arglist);
	va_end(arglist);

	if (mListBox->GetCount() == 10)
	{
		mListBox->Remove(mListBox->GetCount() - 1);
	}

	mListBox->Insert(0, text.c_wstr());
}

void Console::OnClose(const MGUI::ClickEvent * e)
{
	SetVisible(false);
}

void Console::OnClear(const MGUI::ClickEvent * e)
{
	mListBox->Clear();
}