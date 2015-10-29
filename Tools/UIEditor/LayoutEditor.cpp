#include "LayoutEditor.h"
#include "MainFrame.h"
#include "App.h"

ImplementSingleton(LayoutEditor);

LayoutEditor::LayoutEditor()
{
	mLayoutPanel = new LayoutPanel;
	mLayoutProperty = new LayoutProperty;
	mLayoutViewEd = new LayoutViewEd;

	mEditLayout = NULL;
	mEditWidget = NULL;

	MainFrame::Instance()->E_EditorModeChanged += new cListener1<LayoutEditor, int>(this, &LayoutEditor::OnEditorModeChanged_);
}

LayoutEditor::~LayoutEditor()
{
	E_EditWidgetChanged.DetachAll();
	E_NameChanged.DetachAll();
	E_RectChanged.DetachAll();

	delete mLayoutViewEd;
	delete mLayoutProperty;
	delete mLayoutPanel;
}

void LayoutEditor::Layout()
{
	mLayoutPanel->Layout();
	mLayoutProperty->Layout();
	mLayoutViewEd->Layout();
}

void LayoutEditor::_createLayout(const MGUI::LookFeel * _lk)
{
	_distroyLayout();

	mEditLayout = new MGUI::Layout(_lk, mLayoutViewEd->GetLayout());
	mEditLayout->SetName("Root");

	if (_lk == NULL)
		mEditLayout->SetRect(0, 0, 256, 256);
	
	notifyEditWidgetChanged(mEditLayout);
}

void LayoutEditor::_distroyLayout()
{
	mLayoutPanel->Clear();

	safe_delete (mEditLayout);
	notifyEditWidgetChanged(NULL);

	mFilename = "";

	_setLayoutFile(mFilename);
}

void LayoutEditor::notifyEditWidgetChanged(MGUI::Widget * _widget)
{
	if (mEditWidget != _widget)
	{
		mEditWidget = _widget;

		E_EditWidgetChanged(_widget);
		
		if (_widget)
		{
			MGUI::Rect myRect = _widget->GetRect();

			notifyRectChanged(myRect);
		}
	}
}

void LayoutEditor::notifyNameChanged(String _name)
{
	if (mEditWidget)
	{
		if (_name == "" || _name.c_str() == mEditLayout->GetName())
			return ;

		mEditWidget->SetName(_name.c_str());
		E_NameChanged(_name);
	}
}

void LayoutEditor::notifyRectChanged(MGUI::Rect _rect)
{
	if (mEditWidget)
	{
		mEditWidget->SetRect(_rect);

		E_RectChanged(_rect);
	}
}

void LayoutEditor::Open()
{
	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner =GetForegroundWindow();
	ofn.lpstrFilter = TEXT("layout(*.layout)\0*.layout\0");
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
		_distroyLayout();

		mFilename = szPathName;

		DataStreamPtr stream = ResourceManager::Instance()->OpenFile(mFilename);

		MGUI::Loader loader;
		loader.E_WidgetLoaded += new cListener1<LayoutPanel, MGUI::Widget *>(mLayoutPanel, &LayoutPanel::OnWidgetLoaded);

		MGUI::Layout * layout = loader.Load(stream, mLayoutViewEd->GetLayout());
		if (layout == NULL)
		{
			MessageBox(App::Instance()->_hWnd(), "Layout Open Failed!", "Error", MB_OK | MB_ICONERROR);
			return ;
		}

		mEditLayout = layout;

		_setLayoutFile(szPathName);

		notifyEditWidgetChanged(layout);
	}
}

void LayoutEditor::Save()
{
	if (mFilename != "")
	{
		mLayoutPanel->SaveLayout(mFilename);
		return ;
	}

	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner = GetForegroundWindow();
	ofn.lpstrFilter = TEXT("layout(*.layout)\0*.layout\0"); 
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

		if (FileHelper::GetExternName(mFilename) != "layout")
		{
			mFilename += ".layout";
		}

		_setLayoutFile(mFilename);

		mLayoutPanel->SaveLayout(mFilename);
	}
}

void LayoutEditor::_setLayoutFile(const String & filename)
{
	mFilename = filename;

	if (MainFrame::Instance()->GetEditorMode() == EM_Layout)
		MainStatusBar::Instance()->SetText(mFilename);
}

void LayoutEditor::OnEditorModeChanged_(int _mode)
{
	if (_mode == EM_Layout)
	{
		mLayoutPanel->GetLayout()->SetVisible(true);
		mLayoutProperty->GetLayout()->SetVisible(true);
		mLayoutViewEd->GetLayout()->SetVisible(true);

		if (mFilename != "")
			_setLayoutFile(mFilename);
	}
	else
	{
		mLayoutPanel->GetLayout()->SetVisible(false);
		mLayoutProperty->GetLayout()->SetVisible(false);
		mLayoutViewEd->GetLayout()->SetVisible(false);
	}
}
