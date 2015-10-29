/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MeshExporter.h"

class AnimExporter : MeshExporter
{
public:
	AnimExporter(IGameScene * gs);
	~AnimExporter();

	void 
		Export();
};