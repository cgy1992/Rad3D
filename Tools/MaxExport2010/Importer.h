/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

class ImportClassDesc : public ClassDesc {
public:
	int IsPublic();
	void * Create(BOOL loading = FALSE);
	const TCHAR * ClassName();
	SClass_ID SuperClassID();
	Class_ID ClassID();
	const TCHAR* Category();
};


class MaxImport : public SceneImport
{
public:
	MaxImport();
	virtual ~MaxImport();

	virtual int DoImport(const TCHAR *name,ImpInterface *ii,Interface *i, BOOL suppressPrompts);

	virtual int ExtCount();	
	virtual const TCHAR * Ext(int n);
	virtual const TCHAR * ShortDesc();
	virtual const TCHAR * LongDesc();
	virtual const TCHAR * AuthorName();
	virtual const TCHAR * CopyrightMessage();
	virtual const TCHAR * OtherMessage1();
	virtual const TCHAR * OtherMessage2();
	virtual unsigned int Version();
	virtual void ShowAbout(HWND hWnd);
	virtual BOOL SupportsOptions(int ext, DWORD options);
};