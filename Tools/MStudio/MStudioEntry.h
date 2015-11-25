/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#ifdef MSTUDIO_EXPORT
#define MSTUDIO_ENTRY __declspec(dllexport)
#else
#define MSTUDIO_ENTRY __declspec(dllimport)
#endif

#include <MBase.h>
#include <MGUI.h>
#include <MWorld.h>
#include <ParticleFX.h>
#include <PhyWorld.h>
#include <GLRenderSystem.h>
#include <ALAudioSystem.h>
#include <DIInputSystem.h>

using namespace Rad;

#define INDENT_TIME 0.2f