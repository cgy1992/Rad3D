#include "stdafx.h"
#include "ExportConfig.h"
#include <fstream>

extern HINSTANCE ghInstance;

ImplementSingleton(ExportConfig);

ExportConfig gConfig;

ExportConfig::ExportConfig()
{
	mExportSelected = false;

	mExportNormal = true;
	mExportTangent = false;
	mExportColor = false;
	mExportTexcoord = true;
	mExportLightmapUV = false;

	mExportAnimation = false;

	mExportAnimName = "Default";
}

ExportConfig::~ExportConfig()
{
}

void ExportConfig::Load()
{
	char path[MAX_PATH];

	GetModuleFileName(ghInstance, path, MAX_PATH);

	String dir = FileHelper::GetFileDir(path);

	String filename = dir + "\\MaxExport.ini";

	xml_doc xdoc;
	if (xdoc.open_file(filename))
	{
		xml_node root = xdoc.first_node();

		xml_node ndExportSelected = root.first_node("ExportSelected");
		xml_node ndExportNormal = root.first_node("ExportNormal");
		xml_node ndExportTangent = root.first_node("ExportTangent");
		xml_node ndExportColor = root.first_node("ExportColor");
		xml_node ndExportTexcoord = root.first_node("ExportTexcoord");
		xml_node ndExportLightmapUV = root.first_node("ExportLightmapUV");
		xml_node ndExportAnimation = root.first_node("ExportAnimation");

		if (ndExportSelected != NULL)
		{
			String val = ndExportSelected.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportSelected = true;
		}

		if (ndExportNormal != NULL)
		{
			String val = ndExportNormal.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportNormal = true;
		}

		if (ndExportTangent != NULL)
		{
			String val = ndExportTangent.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportTangent = true;
		}

		if (ndExportColor != NULL)
		{
			String val = ndExportColor.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportColor = true;
		}

		if (ndExportTexcoord != NULL)
		{
			String val = ndExportTexcoord.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportTexcoord = true;
		}

		if (ndExportLightmapUV != NULL)
		{
			String val = ndExportLightmapUV.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportLightmapUV = true;
		}

		if (ndExportAnimation != NULL)
		{
			String val = ndExportAnimation.value();

			if (stricmp(val.c_str(), "true") == 0)
				mExportAnimation = true;
		}
	}
}

void ExportConfig::Save()
{
	char path[MAX_PATH];

	GetModuleFileName(ghInstance, path, MAX_PATH);

	String dir = FileHelper::GetFileDir(path);

	String filename = dir + "\\MaxExport.ini";

	std::ofstream stream;

	stream.open(filename.c_str());

	d_assert (stream.is_open());

	stream << "<Config>" << std::endl;
	{
		stream << "    " << "<ExportSelected>" << (mExportSelected ? "true" : "false") << "</ExportSelected>" << std::endl;
		stream << "    " << "<ExportNormal>" << (mExportNormal ? "true" : "false") << "</ExportNormal>" << std::endl;
		stream << "    " << "<ExportTangent>" << (mExportTangent ? "true" : "false") << "</ExportTangent>" << std::endl;
		stream << "    " << "<ExportColor>" << (mExportColor ? "true" : "false") << "</ExportColor>" << std::endl;
		stream << "    " << "<ExportTexcoord>" << (mExportTexcoord ? "true" : "false") << "</ExportTexcoord>" << std::endl;
		stream << "    " << "<ExportLightmapUV>" << (mExportLightmapUV ? "true" : "false") << "</ExportLightmapUV>" << std::endl;
		stream << "    " << "<ExportAnimation>" << (mExportAnimation ? "true" : "false") << "</ExportAnimation>" << std::endl;
	}
	stream << "</Config>" << std::endl;

	stream.close();
}

void ExportConfig::SetExportFilename(const String & filename)
{
	mExportFilename = filename.c_str();
	mExportFilename.Replace('\\', '/');

	String temp = mExportFilename.c_str();
	String base, path;
	temp.SplitFileName(base, path);

	mExportBaseName = base.c_str();
	mExportPath = path.c_str();
}
