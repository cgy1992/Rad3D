#include "stdafx.h"

#include "TextureExporter.h"
#include "ExportConfig.h"

ImplementSingleton(TextureExporter);

TextureExporter::TextureExporter()
{
}

TextureExporter::~TextureExporter()
{
}

void TextureExporter::Clear()
{
	mTextures.Clear();
}

void TextureExporter::Push(const char * texture)
{
	mTextures.Insert(texture);
}

void TextureExporter::Export()
{
	String expPath = ExportConfig::Instance()->GetExportPath();

	for (int i = 0; i < mTextures.Size(); ++i)
	{
		String filename = mTextures[i].c_str();

		String basename = FileHelper::GetBaseName(filename);

		String newFilename = expPath + "/" + basename;

		CopyFile(filename.c_str(), newFilename.c_str(), FALSE);
	}
}