#include "AllLookFeel.h"

namespace Rad {

	ImplementSingleton(AllLookFeel);

	AllLookFeel::AllLookFeel()
	{
		const char * skinName = "Editor.png";

		// Menu
		mMenu = new MGUI::LookFeel("MainMenu");
		mMenu->SetSkin(skinName);
		mMenu->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(202, 2, 27, 26));
		mMenu->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(202, 2, 27, 26));
		mMenu->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(202, 2, 27, 26));
		mMenu->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(202, 2, 27, 26));
		mMenu->SetClientRect(MGUI::ClientRect(6, 0, 6, 0));

		// Button
		mButton = new MGUI::LookFeel("Button");
		mButton->SetSkin(skinName);
		mButton->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(112, 198, 18, 14));
		mButton->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(112, 183, 18, 14));
		mButton->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(112, 213, 18, 14));
		mButton->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(112, 228, 18, 14));
		mButton->SetClientRect(MGUI::ClientRect(6, 4, 6, 4));

		// Panel
		mPanel =  new MGUI::LookFeel("Panel");
		mPanel->SetSkin(skinName);
		mPanel->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 2, 23, 22));
		mPanel->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 2, 23, 22));
		mPanel->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 2, 23, 22));
		mPanel->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 2, 23, 22));
		mPanel->SetClientRect(MGUI::ClientRect(8, 8, 8, 8));

		mWhite =  new MGUI::LookFeel("White");
		mWhite->SetSkin(skinName);
		mWhite->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(454, 6, 2, 2));
		mWhite->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(454, 6, 2, 2));
		mWhite->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(454, 6, 2, 2));
		mWhite->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(454, 6, 2, 2));
		mWhite->SetClientRect(MGUI::ClientRect(0, 0, 0, 0));

		// scroll v
		MGUI::LookFeel * mBnVScrollUp = new MGUI::LookFeel("ScrollUp");
		mBnVScrollUp->SetSkin(skinName);
		mBnVScrollUp->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(95, 68, 14, 11));
		mBnVScrollUp->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(95, 54, 14, 11));
		mBnVScrollUp->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(95, 82, 14, 11));
		mBnVScrollUp->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(95, 96, 14, 11));
		mBnVScrollUp->SetClientRect(MGUI::ClientRect(0, 0, 0, 0));

		MGUI::LookFeel * mBnVScrollDown = new MGUI::LookFeel("ScrollDown");
		mBnVScrollDown->SetSkin(skinName);
		mBnVScrollDown->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(95, 15, 14, 11));
		mBnVScrollDown->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(95, 2, 14, 11));
		mBnVScrollDown->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(95, 28, 14, 11));
		mBnVScrollDown->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(95, 41, 14, 11));
		mBnVScrollDown->SetClientRect(MGUI::ClientRect(0, 0, 0, 0));

		MGUI::LookFeel * mBnVScroll = new MGUI::LookFeel("ScrollTrack");
		mBnVScroll->SetSkin(skinName);
		mBnVScroll->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(113, 57, 13, 18));
		mBnVScroll->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(113, 37, 13, 18));
		mBnVScroll->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(113, 77, 13, 18));
		mBnVScroll->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(113, 97, 13, 18));
		mBnVScroll->SetClientRect(MGUI::ClientRect(6, 6, 6, 6));

		mVScrollBar = new MGUI::LookFeel("VScrollBar");
		mVScrollBar->SetSkin(skinName);
		mVScrollBar->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 73, 15,  19));
		mVScrollBar->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 73, 15,  19));
		mVScrollBar->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 73, 15,  19));
		mVScrollBar->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 73, 15,  19));
		mVScrollBar->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));
		mVScrollBar->AddChild(mBnVScroll);
		mVScrollBar->AddChild(mBnVScrollUp);
		mVScrollBar->AddChild(mBnVScrollDown);

		// scroll h
		MGUI::LookFeel * mBnHScrollLeft = new MGUI::LookFeel("ScrollLeft");
		mBnHScrollLeft->SetSkin(skinName);
		mBnHScrollLeft->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(95, 130, 11, 13));
		mBnHScrollLeft->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(95, 114, 11, 13));
		mBnHScrollLeft->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(95, 146, 11, 13));
		mBnHScrollLeft->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(95, 162, 11, 13));
		mBnHScrollLeft->SetClientRect(MGUI::ClientRect(0, 0, 0, 0));

		MGUI::LookFeel * mBnHScrollRight = new MGUI::LookFeel("ScrollRight");
		mBnHScrollRight->SetSkin(skinName);
		mBnHScrollRight->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(95, 196, 11, 13));
		mBnHScrollRight->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(95, 180, 11, 13));
		mBnHScrollRight->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(95, 212, 11, 13));
		mBnHScrollRight->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(95, 228, 11, 13));
		mBnHScrollRight->SetClientRect(MGUI::ClientRect(0, 0, 11, 13));

		MGUI::LookFeel * mBnHScroll = new MGUI::LookFeel("ScrollTrack");
		mBnHScroll->SetSkin(skinName);
		mBnHScroll->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(113, 198, 18, 13));
		mBnHScroll->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(113, 183, 18, 13));
		mBnHScroll->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(113, 213, 18, 13));
		mBnHScroll->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(113, 228, 18, 13));
		mBnHScroll->SetClientRect(MGUI::ClientRect(6, 6, 6, 6));

		mHScrollBar = new MGUI::LookFeel("HScrollBar");
		mHScrollBar->SetSkin(skinName);
		mHScrollBar->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 54, 18, 14));
		mHScrollBar->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 54, 18, 14));
		mHScrollBar->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 54, 18, 14));
		mHScrollBar->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 54, 18, 14));
		mHScrollBar->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));
		mVScrollBar->AddChild(mBnHScroll);
		mVScrollBar->AddChild(mBnHScrollLeft);
		mVScrollBar->AddChild(mBnHScrollRight);

		// ListBox
		mListBoxItem = new MGUI::LookFeel("ListBoxItem");
		mListBoxItem->SetSkin(skinName);
		mListBoxItem->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(204, 160, 42, 9));
		mListBoxItem->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(204, 160, 42, 9));
		mListBoxItem->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(204, 174, 42, 9));
		mListBoxItem->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(204, 188, 42, 9));
		mListBoxItem->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(204, 188, 42, 9));
		mListBoxItem->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));

		mListBox = new MGUI::LookFeel("ListBox");
		mListBox->SetSkin(skinName);
		mListBox->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 2, 22, 21));
		mListBox->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 2, 22, 21));
		mListBox->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 2, 22, 21));
		mListBox->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 2, 22, 21));
		mListBox->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(341, 2, 22, 21));
		mListBox->SetClientRect(MGUI::ClientRect(6, 6, 6, 6));
		mListBox->AddChild(mListBoxItem->Clone("ListBoxItem"));
		mListBox->AddChild(mVScrollBar->Clone("VScrollBar"));

		// TreeView
		MGUI::LookFeel * pTreeViewExpand = new MGUI::LookFeel("TreeViewExpand");
		pTreeViewExpand->SetSkin(skinName);
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(94, 194, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(94, 178, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(94, 210, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(94, 226, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(94, 11, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::SELECT_DISABLE, MGUI::Rect(94, -1, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::SELECT_FOCUS, MGUI::Rect(94, 24, 14, 16));
		pTreeViewExpand->SetUVRect(MGUI::eWidgetState::SELECT_PRESS, MGUI::Rect(94, 38, 14, 16));

		MGUI::LookFeel * pTreeViewItem = new MGUI::LookFeel("TreeViewItem");
		pTreeViewItem->SetSkin(skinName);
		pTreeViewItem->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(204, 160, 42, 9));
		pTreeViewItem->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(204, 160, 42, 9));
		pTreeViewItem->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(204, 160, 42, 9));
		pTreeViewItem->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(204, 188, 42, 9));
		pTreeViewItem->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(204, 188, 42, 9));
		pTreeViewItem->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));

		mTreeView = new MGUI::LookFeel("TreeView");
		mTreeView->SetSkin(skinName);
		mTreeView->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 2, 22, 21));
		mTreeView->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 2, 22, 21));
		mTreeView->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 2, 22, 21));
		mTreeView->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 2, 22, 21));
		mTreeView->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(341, 2, 22, 21));
		mTreeView->SetClientRect(MGUI::ClientRect(6, 6, 6, 6));
		mTreeView->AddChild(pTreeViewExpand);
		mTreeView->AddChild(pTreeViewItem);
		mTreeView->AddChild(mVScrollBar->Clone("VScrollBar"));

		// TabControl
		MGUI::LookFeel * pTabItem = new MGUI::LookFeel("Page");
		pTabItem->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(429, 182, 51, 26));
		pTabItem->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(429, 182, 51, 26));
		pTabItem->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(429, 182, 51, 26));
		pTabItem->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(429, 182, 51, 26));
		pTabItem->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(378, 182, 51, 26));
		pTabItem->SetClientRect(MGUI::ClientRect(12, 4, 12, 4));

		MGUI::LookFeel * pTabPage = new MGUI::LookFeel("Page");
		pTabPage->SetUVRect(-1, MGUI::Rect(358, 213, 43, 40));

		mTabControl = new MGUI::LookFeel("TabControl");
		mTabControl->SetUVRect(-1, MGUI::Rect(0, 0, 1, 1));
		mTabControl->AddChild(pTabItem);
		mTabControl->AddChild(pTabPage);

		// slider
		MGUI::LookFeel * mBnVSliderTrack = new MGUI::LookFeel("SliderTrack");
		mBnVSliderTrack->SetSkin(skinName);
		mBnVSliderTrack->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(113, 10,  13, 6));
		mBnVSliderTrack->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(113, 2,  13, 6));
		mBnVSliderTrack->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(113, 18,  13, 6));
		mBnVSliderTrack->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(113, 26,  13, 6));
		mBnVSliderTrack->SetClientRect(MGUI::ClientRect(4, 0, 4, 0));

		mVSlider = new MGUI::LookFeel("VSlider");
		mVSlider->SetSkin(skinName);
		mVSlider->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 73, 15,  19));
		mVSlider->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 73, 15,  19));
		mVSlider->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 73, 15,  19));
		mVSlider->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 73, 15,  19));
		mVSlider->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));
		mVSlider->AddChild(mBnVSliderTrack);

		MGUI::LookFeel * mBnHSliderTrack = new MGUI::LookFeel("SliderTrack");
		mBnHSliderTrack->SetSkin(skinName);
		mBnHSliderTrack->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(113, 135,  6, 13));
		mBnHSliderTrack->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(113, 120,  6, 13));
		mBnHSliderTrack->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(113, 150,  6, 13));
		mBnHSliderTrack->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(113, 165,  6, 13));
		mBnHSliderTrack->SetClientRect(MGUI::ClientRect(0, 4, 0, 4));

		mHSlider = new MGUI::LookFeel("HSlider");
		mHSlider->SetSkin(skinName);
		mHSlider->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 54, 18,  14));
		mHSlider->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 54, 18,  14));
		mHSlider->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 54, 18,  14));
		mHSlider->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 54, 18,  14));
		mHSlider->SetClientRect(MGUI::ClientRect(2, 2, 2, 2));
		mHSlider->AddChild(mBnHSliderTrack);

		
		//ProgressBar
		MGUI::LookFeel * mProgressBarTrack = new MGUI::LookFeel("Track");
		mProgressBarTrack->SetSkin(skinName);
		mProgressBarTrack->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(113, 135, 6, 13));
		mProgressBarTrack->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(113, 120, 6, 13));
		mProgressBarTrack->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(113, 135, 6, 13));
		mProgressBarTrack->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(113, 135, 6, 13));
		mProgressBarTrack->SetClientRect(MGUI::ClientRect(4, 2, 4, 2));

		mProgressBar = new MGUI::LookFeel("ProgressBar");
		mProgressBar->SetSkin(skinName);
		mProgressBar->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(341, 54, 18,  14));
		mProgressBar->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(341, 54, 18,  14));
		mProgressBar->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(341, 54, 18,  14));
		mProgressBar->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(341, 54, 18,  14));
		mProgressBar->SetClientRect(MGUI::ClientRect(4, 2, 4, 2));
		mProgressBar->AddChild(mProgressBarTrack);
		
		// EditBox
		mEditBox = new MGUI::LookFeel("EditBox");
		mEditBox->SetSkin(skinName);
		mEditBox->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(168, 28, 30, 27));
		mEditBox->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(168, 1, 30, 27));
		mEditBox->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(168, 55, 30, 27));
		mEditBox->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(168, 136, 30, 27));
		mEditBox->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(168, 136, 30, 27));
		mEditBox->SetClientRect(MGUI::ClientRect(5, 5, 5, 5));

		// CheckBox
		mCheckBox = new MGUI::LookFeel("CheckBox");
		mCheckBox->SetSkin(skinName);
		mCheckBox->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(0, 23, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(0, 0, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(0, 45, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(0, 45, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(0, 111, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::SELECT_DISABLE, MGUI::Rect(0, 89, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::SELECT_FOCUS, MGUI::Rect(0, 133, 24, 23));
		mCheckBox->SetUVRect(MGUI::eWidgetState::SELECT_PRESS, MGUI::Rect(0, 155, 24, 23));

		// ComboBox
		mComboBox = new MGUI::LookFeel("ComboxBox");
		mComboBox->AddChild(mEditBox->Clone("EditBox"));
		mComboBox->AddChild(mListBox->Clone("ListBox"));
		mComboBox->AddChild(mBnVScrollDown->Clone("DropButton"));

		// Dialog
		mDialog = new MGUI::LookFeel("Dialog");
		mDialog->SetSkin(skinName);
		mDialog->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(296, 197, 21, 20));
		mDialog->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(296, 197, 21, 20));
		mDialog->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(296, 197, 21, 20));
		mDialog->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(296, 197, 21, 20));
		mDialog->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(296, 197, 21, 20));
		mDialog->SetClientRect(MGUI::ClientRect(5, 4, 5, 4));

		mDialogCaption = new MGUI::LookFeel("Dialog");
		mDialogCaption->SetSkin(skinName);
		mDialogCaption->SetUVRect(MGUI::eWidgetState::NORMAL, MGUI::Rect(268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI::eWidgetState::DISABLE, MGUI::Rect(268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI::eWidgetState::FOCUS, MGUI::Rect(268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI::eWidgetState::PRESS, MGUI::Rect(268, 36, 68, 28));
		mDialogCaption->SetUVRect(MGUI::eWidgetState::SELECT, MGUI::Rect(268, 36, 68, 28));
		mDialogCaption->SetClientRect(MGUI::ClientRect(16, 0, 16, 0));

		//
		mHelperTexture = HWBufferManager::Instance()->NewTexture("Editor_HelperTexture", 4, 4);
		int * pPixelData = (int *)mHelperTexture->Lock(eLockFlag::WRITE);
		for (int i = 0; i < 4 * 4; ++i)
		{
			*pPixelData++ = M_RGBA(0, 0, 0, 0x30);
		}
		mHelperTexture->Unlock();

		mEditorHelper  = new MGUI::LookFeel("EditorBlack");
		mEditorHelper->SetSkin(mHelperTexture->GetName());
		mEditorHelper->SetUVRect(-1, MGUI::Rect(0, 0, 1, 1));
	}

	AllLookFeel::~AllLookFeel()
	{
		delete mMenu;

		delete mButton;

		delete mPanel;
		delete mWhite;

		delete mVScrollBar;
		delete mHScrollBar;
		
		delete mListBoxItem;
		delete mListBox;

		delete mTreeView;

		delete mTabControl;

		delete mVSlider;
		delete mHSlider;

		delete mProgressBar;

		delete mEditBox;

		delete mCheckBox;

		delete mComboBox;

		delete mDialog;
		delete mDialogCaption;

		delete mEditorHelper;
	}

	void AllLookFeel::InitEditorHelper()
	{
		MGUI::LookFeelManager::Instance()->SetDefaultLookfeel(mEditorHelper);
	}

}