/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MCore.h"
#include "MTypes.h"
#include "MRenderSystem.h"

#ifdef M_PLATFORM_WIN32

#include <windows.h>
#include <glew.h>
#include <glut.h>
#include <wglew.h>

#endif

#ifdef M_PLATFORM_ANDROID

#include <GLES2/gl2.h>  
#include <GLES2/gl2ext.h>

#define glClearDepth glClearDepthf
#define glOrtho glOrthof
#define glFrustum glFrustumf

#endif

