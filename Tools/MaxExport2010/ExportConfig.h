/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "stdafx.h"

class ExportConfig : public Singleton<ExportConfig>
{
public:
	ExportConfig();
	~ExportConfig();

	void 
		Load();
	void 
		Save();

	void 
		SetExportFilename(const String & filename);

	String 
		GetExportFilename() const { return mExportFilename.c_str(); }
	String
		GetExportPath() const { return mExportPath.c_str(); }
	String 
		GetExportBaseName() const { return mExportBaseName.c_str(); }
	String
		GetExportAnimName() const { return mExportAnimName.c_str(); }

	void 
		SetExportSelected(bool b) { mExportSelected = b; }
	bool 
		IsExportSelected() const { return mExportSelected; }

	void 
		SetExportNormal(bool b) { mExportNormal = b; }
	bool 
		IsExportNormal() const { return mExportNormal; }

	void 
		SetExportTangent(bool b) { mExportTangent = b; }
	bool 
		IsExportTangent() const { return mExportTangent; }

	void 
		SetExportColor(bool b) { mExportColor = b; }
	bool 
		IsExportColor() const { return mExportColor; }

	void 
		SetExportTexcoord(bool b) { mExportTexcoord = b; }
	bool 
		IsExportTexcoord() const { return mExportTexcoord; }

	void 
		SetExportLightmapUV(bool b) { mExportLightmapUV = b; }
	bool 
		IsExportLightmapUV() const { return mExportLightmapUV; }

	void 
		SetExportAnimation(bool b) { mExportAnimation = b; }
	bool 
		IsExportAnimation() const { return mExportAnimation; }

	void
		SetFrameRange(int start, int end) { mFrameRange.x = start; mFrameRange.y = end; }
	Int2
		GetFrameRange() const { return mFrameRange; }
	int
		GetFrames() const { return mFrameRange.y - mFrameRange.x; }

protected:
	FixedString256 mExportFilename;
	FixedString256 mExportPath;
	FixedString256 mExportBaseName;
	FixedString256 mExportAnimName;

	bool mExportSelected;
	bool mExportNormal;
	bool mExportTangent;
	bool mExportColor;
	bool mExportTexcoord;
	bool mExportLightmapUV;

	bool mExportAnimation;

	Int2 mFrameRange;
};