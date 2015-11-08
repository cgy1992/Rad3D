/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MInclude.h"

namespace Rad {

	class AllLookFeel : public Singleton<AllLookFeel>
	{
	public:
		AllLookFeel();
		~AllLookFeel();

		MGUI::LookFeel * GetEditorHelper() { return mEditorHelper; }

		MGUI::LookFeel * GetMenu() { return mMenu; }

		MGUI::LookFeel * GetButton() { return mButton; }

		MGUI::LookFeel * GetPanel() { return mPanel; }
		MGUI::LookFeel * GetWhite() { return mWhite; }

		MGUI::LookFeel * GetVScrollBar() { return mVScrollBar; }
		MGUI::LookFeel * GetHScrollBar() { return mHScrollBar; }

		MGUI::LookFeel * GetListBoxItem() { return mListBoxItem; }
		MGUI::LookFeel * GetListBox() { return mListBox; }

		MGUI::LookFeel * GetTreeView() { return mTreeView; }

		MGUI::LookFeel * GetTabControl() { return mTabControl; }

		MGUI::LookFeel * GetVSlider() { return mVSlider; }
		MGUI::LookFeel * GetHSlider() { return mHSlider; }

		MGUI::LookFeel * GetProgressBar() { return mProgressBar; }

		MGUI::LookFeel * GetEditBox() { return mEditBox; }

		MGUI::LookFeel * GetCheckBox() { return mCheckBox; }

		MGUI::LookFeel * GetComboBox() { return mComboBox; }

		MGUI::LookFeel * GetDialog() { return mDialog; }
		MGUI::LookFeel * GetDialogCaption() { return mDialogCaption; }

		void InitEditorHelper();

	protected:
		MGUI::LookFeel * mMenu;

		MGUI::LookFeel * mButton;
		
		MGUI::LookFeel * mPanel;
		MGUI::LookFeel * mWhite;

		MGUI::LookFeel * mVScrollBar;
		MGUI::LookFeel * mHScrollBar;

		MGUI::LookFeel * mListBoxItem;
		MGUI::LookFeel * mListBox;

		MGUI::LookFeel * mTreeView;
		MGUI::LookFeel * mTabControl;

		MGUI::LookFeel * mVSlider;
		MGUI::LookFeel * mHSlider;

		MGUI::LookFeel * mProgressBar;

		MGUI::LookFeel * mEditBox;

		MGUI::LookFeel * mCheckBox;

		MGUI::LookFeel * mComboBox;

		MGUI::LookFeel * mDialog;
		MGUI::LookFeel * mDialogCaption;

		MGUI::LookFeel * mEditorHelper;
		TexturePtr mHelperTexture;
	};

}