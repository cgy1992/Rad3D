#include "ToolUtil.h"
#include <shlobj.h>

bool Util_QuestDlg(HWND hwnd, const char * caption)
{
	return MessageBox(hwnd, caption, "Quest", MB_OKCANCEL) == IDOK;
}

String Util_GetOpenFile(const char * filter, const String & path)
{
	static FixedString256 S_FILEDIR = "";

	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner = GetForegroundWindow();
	ofn.lpstrFilter = filter;
	lstrcpy(szPathName, "");
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);
	ofn.lpstrTitle = TEXT("选择文件");
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	TCHAR szCurDir[MAX_PATH] = { 0 };
	if (path != "")
	{
		if (path[0] == '.')
		{
			GetCurrentDirectory(sizeof(szCurDir),szCurDir);

			String abspath = FileHelper::Transliterate(szCurDir, path);
			strcpy(szCurDir, abspath.c_str());
		}
		else
		{
			strcpy(szCurDir, path.c_str());
		}
	}
	else if (S_FILEDIR != "")
	{
		strcpy(szCurDir, S_FILEDIR.c_str());
	}
	else
	{
		GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	}

	for (int i = 0; szCurDir[i] != 0; ++i)
	{
		if (szCurDir[i] == '/')
		{
			szCurDir[i] = '\\';
		}
	}

	ofn.lpstrInitialDir=szCurDir;

	if (GetOpenFileName(&ofn))
	{
		S_FILEDIR = FileHelper::GetFileDir(szPathName).c_str();

		return szPathName;
	}

	return "";
}

String Util_GetSaveFile(const String & path, const char * filter)
{
	static FixedString256 S_FILEDIR = "";

	TCHAR szPathName[MAX_PATH];  
	OPENFILENAME ofn = { OPENFILENAME_SIZE_VERSION_400 };
	ofn.hwndOwner = GetForegroundWindow();
	ofn.lpstrFilter = filter; 
	lstrcpy(szPathName, "");  
	ofn.lpstrFile = szPathName;  
	ofn.nMaxFile = sizeof(szPathName);
	ofn.lpstrTitle = TEXT("选择文件");
	ofn.Flags = OFN_EXPLORER | OFN_NOCHANGEDIR;

	TCHAR szCurDir[MAX_PATH];  
	if (path != "")
	{
		strcpy(szCurDir, path.c_str());
	}
	else if (S_FILEDIR != "")
	{
		strcpy(szCurDir, S_FILEDIR.c_str());
	}
	else
	{
		GetCurrentDirectory(sizeof(szCurDir),szCurDir);  
	}
	ofn.lpstrInitialDir=szCurDir;

	if (GetSaveFileName(&ofn))
	{
		S_FILEDIR = FileHelper::GetFileDir(szPathName).c_str();

		return szPathName;
	}

	return "";
}

int CALLBACK Util_BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam,LPARAM lpData)
{
	if(uMsg == BFFM_INITIALIZED)
	{
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

String Util_GetOpenFloder()
{
	static FixedString256 S_DIR = "";

	TCHAR szBuffer[MAX_PATH] = {0};
	BROWSEINFO bi;
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szBuffer;
	bi.lpszTitle = NULL;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	if (S_DIR != "")
	{
		bi.lParam = (long)(S_DIR.c_str());
		bi.lpfn = Util_BrowseCallbackProc;
	}

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (idl != NULL)   
	{   
		SHGetPathFromIDList(idl,szBuffer);
	}

	S_DIR = szBuffer;

	return szBuffer;
}

void Util_StartEXE(const char * command)
{
	WinExec(command, 5);
}
