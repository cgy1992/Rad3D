#include "MGUI_Clipboard.h"

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(Clipboard);

	Clipboard::Clipboard()
	{
	}

	Clipboard::~Clipboard()
	{
	}

	void Clipboard::SetText(const UString & _text)
	{
#ifdef M_PLATFORM_WIN32
		HWND hWnd = GetForegroundWindow();

		if (OpenClipboard(hWnd))
		{
			int size = (_text.Length() + 1) * sizeof(uchar_t);

			EmptyClipboard();
			HGLOBAL hgBuffer = GlobalAlloc(GMEM_DDESHARE, size);
			uchar_t* chBuffer = hgBuffer ? (uchar_t*)GlobalLock(hgBuffer) : NULL;

			if (chBuffer)
			{
				memcpy(chBuffer, _text.c_str(), size);
				GlobalUnlock(hgBuffer);
				SetClipboardData(CF_UNICODETEXT, hgBuffer);
			}

			CloseClipboard();
		}
#else
		mText = _text;
#endif
	}

	UString Clipboard::GetText()
	{
#ifdef M_PLATFORM_WIN32
		HWND hWnd = GetForegroundWindow();

		UString text;

		if (OpenClipboard(hWnd))
		{
			HANDLE hData = GetClipboardData(CF_UNICODETEXT);
			uchar_t* chBuffer = hData ? (uchar_t*)GlobalLock(hData) : NULL;

			if (chBuffer)
			{
				text = chBuffer;
				GlobalUnlock(hData);
			}

			CloseClipboard();
		}

		return text;
#else
		return mText;
#endif
	}

}