#include "LookFeelEditor.h"
#include "MainFrame.h"

ImplementSingleton(LookFeelEditor);

LookFeelEditor::LookFeelEditor()
	: mEditLookFeel(NULL)
	, mEditSkin(NULL)
{
	mLookFeelPanel = new LookFeelPanel;
	mLookFeelProperty = new LookFeelProperty;
	mLookFeelSkinEd = new LookFeelSkinEd;

	MainFrame::Instance()->E_EditorModeChanged += new cListener1<LookFeelEditor, int>(this, &LookFeelEditor::OnEditorModeChanged_);
}

LookFeelEditor::~LookFeelEditor()
{
	mEditLookFeel = NULL;
	mEditSkin = NULL;

	delete mLookFeelSkinEd;
	delete mLookFeelProperty;
	delete mLookFeelPanel;
}

void LookFeelEditor::Layout()
{
	mLookFeelPanel->Layout();
	mLookFeelProperty->Layout();
	mLookFeelSkinEd->Layout();
}

void LookFeelEditor::SetEditLookFeel(MGUI::LookFeel * _lookfeel)
{
	if (mEditLookFeel != _lookfeel)
	{
		mEditLookFeel = _lookfeel;
		E_EditLookFeelChanged(_lookfeel);

		if (_lookfeel)
		{
			_notifySkinChanged(HWBufferManager::Instance()->LoadTexture(_lookfeel->GetSkin(), -1).c_ptr());
			_notifyClientChanged(_lookfeel->GetClientRect());
		}
	}
}

int LookFeelEditor::GetEditWidgetType()
{
	return mLookFeelPanel->GetEditWidgetType();
}

MGUI::LookFeel * LookFeelEditor::GetEditLookFeel()
{
	return mEditLookFeel;
}

Texture * LookFeelEditor::GetEditSkin()
{
	return mEditSkin.c_ptr();
}

const String & LookFeelEditor::GetEditSkinName()
{
	return mEditSkin != NULL ? mEditSkin->GetSourceName() : String::null;
}

const String & LookFeelEditor::GetLookFeelFile()
{
	return mFilename;
}

void LookFeelEditor::_notifyNameChanged()
{
	E_NameChanged();
}

void LookFeelEditor::_notifyRectChanged(const MGUI::Rect & _rect)
{
	mEditRect = _rect;

	E_EditRectChanged(_rect);
}

void LookFeelEditor::_notifyClientChanged(const MGUI::ClientRect & _rect)
{
	if (mEditLookFeel)
	{
		mEditLookFeel->SetClientRect(_rect);

		E_EditClientChanged(_rect);
	}
}

void LookFeelEditor::_notifySkinChanged(Texture * _skin)
{
	if (mEditLookFeel)
	{
		if (_skin != NULL)
		{
			mEditLookFeel->SetSkin(_skin->GetSourceName());
		}
		else
		{
			mEditLookFeel->SetSkin("");
		}
	}

	if (mEditSkin != _skin)
	{
		mEditSkin = _skin;
		E_EditSkinChanged(_skin);
	}

	if (mEditSkin != NULL)
	{
		String text;

		text.Format("%s (%d x %d).", mEditSkin->GetSourceName().c_str(), mEditSkin->GetWidth(), mEditSkin->GetHeight());

		MainStatusBar::Instance()->SetText(text);
	}
}

void LookFeelEditor::Open()
{
	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner =GetForegroundWindow();
	ofn.lpstrFilter = TEXT("lookfeel(*.lookfeel)\0*.lookfeel\0");
	lstrcpy(szPathName, "");  
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);
	ofn.lpstrTitle = TEXT("选择文件");
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	TCHAR szCurDir[MAX_PATH];  
	if (mFilename != "")
	{
		String dir = FileHelper::GetFileDir(mFilename);
		strcpy(szCurDir, dir.c_str());
	}
	else
	{
		GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	}
	ofn.lpstrInitialDir=szCurDir;


	if (GetOpenFileName(&ofn))
	{
		SetEditLookFeel(NULL);

		LayoutEditor::Instance()->_distroyLayout();

		MGUI::LookFeelManager::Instance()->Unload(mFilename.c_str());

		mFilename = szPathName;

		DataStreamPtr stream = ResourceManager::Instance()->OpenFile(mFilename);

		MGUI::LookFeelManager::Instance()->LoadFromStream(stream, mFilename.c_str());

		_setLookFeelFile(szPathName);

		E_OpenLookFeel();
	}
}

void LookFeelEditor::Save()
{
	if (mFilename != "")
	{
		MGUI::LookFeelManager::Instance()->Save(mFilename);
		return ;
	}

	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner =GetForegroundWindow();
	ofn.lpstrFilter = TEXT("lookfeel(*.lookfeel)\0*.lookfeel\0"); 
	lstrcpy(szPathName, "");  
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);
	ofn.lpstrTitle = TEXT("选择文件");
	ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR;

	TCHAR szCurDir[MAX_PATH];  
	if (mFilename != "")
	{
		String dir = FileHelper::GetFileDir(mFilename);
		strcpy(szCurDir, dir.c_str());
	}
	else
	{
		GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	}
	ofn.lpstrInitialDir=szCurDir;

	if (GetSaveFileName(&ofn))
	{
		mFilename = szPathName;

		if (FileHelper::GetExternName(mFilename) != "lookfeel")
		{
			mFilename += ".lookfeel";
		}

		_setLookFeelFile(mFilename);

		MGUI::LookFeelManager::Instance()->Save(mFilename);
	}
}

void LookFeelEditor::_setLookFeelFile(const String & filename)
{
	mFilename = filename;

	if (MainFrame::Instance()->GetEditorMode() == EM_Lookfeel)
		MainStatusBar::Instance()->SetText(mFilename);
}

void LookFeelEditor::OnEditorModeChanged_(int _mode)
{
	if (_mode == EM_Lookfeel)
	{
		mLookFeelPanel->GetLayout()->SetVisible(true);
		mLookFeelProperty->GetLayout()->SetVisible(true);
		mLookFeelSkinEd->GetLayout()->SetVisible(true);

		if (mEditSkin != NULL)
		{
			String text;

			text.Format("%s (%d x %d).", mEditSkin->GetSourceName().c_str(), mEditSkin->GetWidth(), mEditSkin->GetHeight());

			MainStatusBar::Instance()->SetText(text);
		}
	}
	else
	{
		mLookFeelPanel->GetLayout()->SetVisible(false);
		mLookFeelProperty->GetLayout()->SetVisible(false);
		mLookFeelSkinEd->GetLayout()->SetVisible(false);
	}
}
