#include "stdafx.h"
#include "Importer.h"

//------------------------------------------------------
int ImportClassDesc::IsPublic()
{
	return 1; 
}

void * ImportClassDesc::Create(BOOL loading)
{ 
	return new MaxImport();
}

const TCHAR * ImportClassDesc::ClassName() 
{ 
	return "Rad Importer"; 
}

SClass_ID ImportClassDesc::SuperClassID() 
{ 
	return SCENE_IMPORT_CLASS_ID; 
}

Class_ID ImportClassDesc::ClassID() 
{ 
	return Class_ID('RadI', 'y3D1'); 
}

const TCHAR* ImportClassDesc::Category()
{ 
	return _T("Rad Importer"); 
}










#include "MeshImporter.h"

MaxImport::MaxImport()
{
}

MaxImport::~MaxImport()
{
}

int	MaxImport::DoImport(const TCHAR *name,ImpInterface *ii,Interface *i, BOOL suppressPrompts)
{
	MeshImporter imp;

	return imp.DoImport(name, i) ? IMPEXP_SUCCESS : IMPEXP_FAIL;
}

int MaxImport::ExtCount()
{
	return 1;
}

const TCHAR * MaxImport::Ext(int n)
{
	switch (n)
	{
	case 0:
		return _T("mesh");
		break;

	case 1:
		return _T("anim");
		break;

	default:
		return 0;
		break;
	}
}

const TCHAR * MaxImport::LongDesc()
{ 
	return _T("Rad Importer");
}

const TCHAR * MaxImport::ShortDesc()
{
	return _T("Rad Importer");
}

const TCHAR * MaxImport::AuthorName()
{ 
	return _T("Rad");
}

const TCHAR * MaxImport::CopyrightMessage()
{ 
	return _T("Slivereyes Information CO. LTM. (c) 2015");
}

const TCHAR * MaxImport::OtherMessage1()
{ 
	return 0;
}

const TCHAR * MaxImport::OtherMessage2()
{ 
	return 0;
}

unsigned int MaxImport::Version()
{ 
	return 1;
}

void MaxImport::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, "Rad Importer", "About", 0);
}

BOOL MaxImport::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}

