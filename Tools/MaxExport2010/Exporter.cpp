#include "stdafx.h"

#include "Exporter.h"
#include "resource.h"

//------------------------------------------------------

int ExportClassDesc::IsPublic()
{
	return 1; 
}

void * ExportClassDesc::Create(BOOL loading)
{ 
	return new MaxExport();
}

const TCHAR * ExportClassDesc::ClassName() 
{ 
	return "Rad Exporter"; 
}

SClass_ID ExportClassDesc::SuperClassID() 
{ 
	return SCENE_EXPORT_CLASS_ID; 
}

Class_ID ExportClassDesc::ClassID() 
{ 
	return Class_ID('RadE', 'y3D0'); 
}

const TCHAR* ExportClassDesc::Category()
{ 
	return _T("Rad Exporter"); 
}




//
#include "ExportConfig.h"
#include "MeshExporter.h"
#include "ExportSettingDlg.h"
#include "TextureExporter.h"
#include "AnimExporter.h"

MaxExport::MaxExport()
{
}

MaxExport::~MaxExport()
{
}

int	MaxExport::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	IGameScene * gs = GetIGameInterface();

	ExportConfig::Instance()->SetExportFilename(name);
	ExportConfig::Instance()->SetFrameRange(
		gs->GetSceneStartTime() / GetTicksPerFrame(),
		gs->GetSceneEndTime() / GetTicksPerFrame());

	ExportSettingDlg dlg;

	int _ok = dlg.DoModal();
	if (_ok == IDOK)
	{
		String ext = FileHelper::GetExternName(name);
		TextureExporter texExp;

		ext.ToLower();
		if (ext == "mesh")
		{
			MeshExporter meshExp(gs);
			meshExp.Export();

			texExp.Export();
		}
		else if (ext == "anim")
		{
			AnimExporter animExp(gs);
			animExp.Export();
		}
	}

	gs->ReleaseIGame();

	return _ok == IDOK ? IMPEXP_SUCCESS : IMPEXP_FAIL;
}

int MaxExport::ExtCount()
{
	return 2;
}

const TCHAR * MaxExport::Ext(int n)
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

const TCHAR * MaxExport::LongDesc()
{ 
	return _T("Rad Exporter");
}

const TCHAR * MaxExport::ShortDesc()
{
	return _T("Rad Exporter");
}

const TCHAR * MaxExport::AuthorName()
{ 
	return _T("Rad");
}

const TCHAR * MaxExport::CopyrightMessage()
{ 
	return _T("Slivereyes Information CO. LTM. (c) 2014");
}

const TCHAR * MaxExport::OtherMessage1()
{ 
	return 0;
}

const TCHAR * MaxExport::OtherMessage2()
{ 
	return 0;
}

unsigned int MaxExport::Version()
{ 
	return 1;
}

void MaxExport::ShowAbout(HWND hWnd)
{
	MessageBox(hWnd, "Rad Exporter", "About", 0);
}

BOOL MaxExport::SupportsOptions(int ext, DWORD options)
{
	return TRUE;
}

