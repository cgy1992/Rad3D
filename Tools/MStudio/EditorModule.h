/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MStudioEntry.h"

class MSTUDIO_ENTRY EditorModule
{
public:
	EditorModule(const String & name);
	virtual ~EditorModule();

	const String 
		GetName() { return mName; }

	virtual void 
		Layout() = 0;
	virtual void 
		Show() = 0;
	virtual void 
		Hide() = 0;

protected:
	String mName;
};