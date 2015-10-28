#include "MGUI_Cursor.h"

namespace Rad {

	using namespace MGUI;

	ImplementSingleton(Cursor);

	Cursor::Cursor()
	{
		mCursor = NULL;
		mLastCursor = NULL;

		Load(kArrow);
		Load(kIbeam);
		Load(kWait);
		Load(kCross);
		Load(kUpArrow);
		Load(kSize);
		Load(kSizeNWSE);
		Load(kSizeNESW);
		Load(kSizeWE);
		Load(kSizeNS);
		Load(kNo);
		Load(kAppStarting);
		Load(kHelp);
		Load(kHand);

		Bind(kArrow);
	}

	Cursor::~Cursor()
	{
		for (int i = 0; i < mCursorMap.Size(); ++i)
		{
			::DestroyCursor(mCursorMap[i].value);
		}
		mCursorMap.Clear();
	}

	void Cursor::Load(int _dwResId)
	{
		HCURSOR hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(_dwResId));

		mCursorMap.Insert(_dwResId, hCursor);
	}

	void Cursor::Bind(int _dwResId)
	{
		int index = mCursorMap.Find(_dwResId);
		if (index != -1)
		{
			mLastCursor = mCursor;
			mCursor = mCursorMap[index].value;
			mCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(_dwResId));
		}
	}

	void Cursor::Unbind()
	{
		mCursor = mLastCursor;
		mLastCursor = NULL;
	}

	void Cursor::Draw()
	{
		::SetCursor(mCursor);
	}

}