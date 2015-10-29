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

		MGUI::LookFeel * GetComboBox() { return mComboBox; }

		MGUI::LookFeel * GetDialog() { return mDialog; }
		MGUI::LookFeel * GetDialogCaption() { return mDialogCaption; }

		void InitEditorHelper();

	protected:
		MGUI::LookFeel * mMenu;

		// Button
		MGUI::LookFeel * mButton;
		
		// panel
		MGUI::LookFeel * mPanel;
		MGUI::LookFeel * mWhite;

		// Scroll
		MGUI::LookFeel * mVScrollBar;
		MGUI::LookFeel * mHScrollBar;

		// ListBox
		MGUI::LookFeel * mListBoxItem;
		MGUI::LookFeel * mListBox;

		// TreeView
		MGUI::LookFeel * mTreeView;

		// TabControl
		MGUI::LookFeel * mTabControl;

		// Slider
		MGUI::LookFeel * mVSlider;
		MGUI::LookFeel * mHSlider;

		// ProgressBar
		MGUI::LookFeel * mProgressBar;

		// EditBox
		MGUI::LookFeel * mEditBox;

		// ComboBox
		MGUI::LookFeel * mComboBox;

		// Dialog
		MGUI::LookFeel * mDialog;
		MGUI::LookFeel * mDialogCaption;

		// EditHelper
		MGUI::LookFeel * mEditorHelper;
		TexturePtr mHelperTexture;
	};

}