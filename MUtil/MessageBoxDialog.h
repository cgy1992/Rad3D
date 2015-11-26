/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MUtil.h"

namespace Rad {

	struct MessageBoxType
	{
		enum {
			Info,
			Asking,
			Error,
			Warning,
		};
	};

	class UTIL_ENTRY MessageBoxDialog : public Singleton<MessageBoxDialog>
	{
	public:
		tEvent1<bool> E_EndDialog;

	public:
		MessageBoxDialog();
		~MessageBoxDialog();

		void DoModal(const String & caption, int type = MessageBoxType::Info, void * userData = NULL);

		void * GetUserData() { return mUserData; }

	protected:
		void OnOK(const MGUI::MouseEvent *);
		void OnCancel(const MGUI::MouseEvent *);

	protected:
		MGUI::Rect mIconRect[4];

		MGUI::Layout * mDialog;
		MGUI::Layout * mLayout;
		MGUI::TextBox * mCaption;
		MGUI::Button * mClose;
		MGUI::Button * mOK;
		MGUI::Button * mCancel;
		MGUI::ImageBox * mIcon;

		void * mUserData;

	};

	inline void DoMessageBox(const String & caption, int type = MessageBoxType::Info, void * userData = NULL)
	{
		MessageBoxDialog::Instance()->DoModal(caption, type, userData);
	}

}

