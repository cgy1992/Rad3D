/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MBase.h"
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

#define M_OPEN_GL_2_0 0x0020
#define M_OPEN_GL_2_1 0x0021
#define M_OPEN_GL_2_2 0x0022
#define M_OPEN_GL_3_0 0x0030
#define M_OPEN_GL_3_1 0x0031
#define M_OPEN_GL_3_2 0x0032
#define M_OPEN_GL_3_3 0x0033
#define M_OPEN_GL_4_0 0x0040
#define M_OPEN_GL_4_1 0x0041
#define M_OPEN_GL_4_2 0x0042
#define M_OPEN_GL_4_3 0x0043

#define M_OPEN_GLES_2_0 0x1020
#define M_OPEN_GLES_3_0 0x1030
