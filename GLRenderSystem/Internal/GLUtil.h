/*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "GLInclude.h"

namespace Rad {

	struct GLUtil
	{
		static int GetPixelFormatBytes(ePixelFormat Format)
		{
			switch (Format)
			{
			case ePixelFormat::R8G8B8A8:
				return 4;

			case ePixelFormat::R8G8B8:
				return 3;

			case ePixelFormat::L8A8:
				return 2;

			case ePixelFormat::L8:
				return 1;

			case ePixelFormat::R5G6B5:
			case ePixelFormat::R5G5B5A1:
			case ePixelFormat::R4G4B4A4:
				return 2;

			case ePixelFormat::R16F:
				return 2;
			case ePixelFormat::R16G16F:
				return 4;
			case ePixelFormat::R16G16B16A16F:
				return 8;

			case ePixelFormat::R32F:
				return 4;
			case ePixelFormat::R32G32F:
				return 8;
			case ePixelFormat::R32G32B32A32F:
				return 16;
			}

			d_assert (0);

			return 0;
		}

		static GLenum GetGLInernalFormat(ePixelFormat Format)
		{
			switch (Format)
			{
			case ePixelFormat::L8:
				return GL_LUMINANCE;

			case ePixelFormat::L8A8:
				return GL_LUMINANCE_ALPHA;

			case ePixelFormat::R8G8B8:
				return GL_RGB;

			case ePixelFormat::R8G8B8A8:
				return GL_RGBA;

			case ePixelFormat::R5G6B5:
				return GL_RGB;

			case ePixelFormat::R5G5B5A1:
				return GL_RGBA;

			case ePixelFormat::R4G4B4A4:
				return GL_RGBA;

#ifdef M_PLATFORM_WIN32
			case ePixelFormat::R16F:
				return GL_R16F;
			case ePixelFormat::R16G16F:
				return GL_RG16F;
			case ePixelFormat::R16G16B16A16F:
				return GL_RGBA16F;

			case ePixelFormat::R32F:
				return GL_R32F;
			case ePixelFormat::R32G32F:
				return GL_RG32F;
			case ePixelFormat::R32G32B32A32F:
				return GL_RGBA32F;
#endif
			}

			d_assert (0);

			return 0;
		}

		static GLenum GetGLPixelFormat(ePixelFormat Format)
		{
			switch (Format)
			{
			case ePixelFormat::L8:
				return GL_LUMINANCE;

			case ePixelFormat::L8A8:
				return GL_LUMINANCE_ALPHA;

			case ePixelFormat::R8G8B8:
				return GL_RGB;

			case ePixelFormat::R8G8B8A8:
				return GL_RGBA;

			case ePixelFormat::R5G6B5:
				return GL_RGB;
			case ePixelFormat::R5G5B5A1:
				return GL_RGBA;
			case ePixelFormat::R4G4B4A4:
				return GL_RGBA;

#ifdef M_PLATFORM_WIN32
			case ePixelFormat::R16F:
				return GL_RED;
			case ePixelFormat::R16G16F:
				return GL_RG;
			case ePixelFormat::R16G16B16A16F:
				return GL_RGBA;

			case ePixelFormat::R32F:
				return GL_RED;
			case ePixelFormat::R32G32F:
				return GL_RG;
			case ePixelFormat::R32G32B32A32F:
				return GL_RGBA;
#endif
			}

			d_assert (0);

			return 0;
		}

		static GLenum GetGLCrompressedPixelFormat(ePixelFormat Format)
		{
			switch (Format)
			{
#ifdef GL_COMPRESSED_RGB_S3TC_DXT1_EXT
			case ePixelFormat::DXT1_RGB:
				return GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
#endif

#ifdef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
			case ePixelFormat::DXT3_RGBA:
				return GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
#endif

#ifdef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
			case ePixelFormat::DXT5_RGBA:
				return GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
#endif

#ifdef GL_ETC1_RGB8_OES
			case ePixelFormat::ETC1_RGB:
				return GL_ETC1_RGB8_OES;
#endif

#ifdef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
			case ePixelFormat::PVRTC_RGB:
				return GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
#endif

#ifdef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
			case ePixelFormat::PVRTC_RGBA:
				return GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
#endif
			}

			d_assert (0);

			return 0;
		}

		static GLenum GetGLPixelType(ePixelFormat Format)
		{
			switch (Format)
			{
			case ePixelFormat::L8:
			case ePixelFormat::L8A8:
			case ePixelFormat::R8G8B8:
			case ePixelFormat::R8G8B8A8:
				return GL_UNSIGNED_BYTE;

			case ePixelFormat::R5G6B5:
				return GL_UNSIGNED_SHORT_5_6_5;

			case ePixelFormat::R5G5B5A1:
				return GL_UNSIGNED_SHORT_5_5_5_1;

			case ePixelFormat::R4G4B4A4:
				return GL_UNSIGNED_SHORT_4_4_4_4;

#ifdef M_PLATFORM_WIN32
			case ePixelFormat::R16F:
			case ePixelFormat::R16G16F:
			case ePixelFormat::R16G16B16A16F:
			case ePixelFormat::R32F:
			case ePixelFormat::R32G32F:
			case ePixelFormat::R32G32B32A32F:
				return GL_FLOAT;
#endif
			}

			d_assert (0);

			return 0;
		}

		static bool IsRenderTargetFormat(ePixelFormat Format)
		{
			return
				Format == ePixelFormat::R8G8B8A8 ||
				Format == ePixelFormat::R8G8B8 ||

				Format == ePixelFormat::R5G6B5 ||
				Format == ePixelFormat::R5G5B5A1 ||
				Format == ePixelFormat::R4G4B4A4 ||

#ifdef M_PLATFORM_WIN32
				Format == ePixelFormat::R16F ||
				Format == ePixelFormat::R16G16F ||
				Format == ePixelFormat::R16G16B16A16F ||

				Format == ePixelFormat::R32F ||
				Format == ePixelFormat::R32G32F ||
				Format == ePixelFormat::R32G32B32A32F ||
#endif

				0;
		}

		static GLenum GetGLPrimType(ePrimType type)
		{
			switch (type)
			{
			case ePrimType::TRIANGLE_LIST:
				return GL_TRIANGLES;

			case ePrimType::TRIANGLE_STRIP:
				return GL_TRIANGLE_STRIP;

			case ePrimType::LINE_LIST:
				return GL_LINES;

			case ePrimType::LINE_STRIP:
				return GL_LINE_STRIP;

			case ePrimType::POINT_LIST:
				return GL_POINTS;
			}

			d_assert (0);

			return GL_TRIANGLES;
		}

		static GLenum GetGLVertType(eVertexType type)
		{
			switch (type)
			{
			case eVertexType::BYTE1:
			case eVertexType::BYTE2:
			case eVertexType::BYTE3:
			case eVertexType::BYTE4:
				return GL_BYTE;

			case eVertexType::UBYTE1:
			case eVertexType::UBYTE2:
			case eVertexType::UBYTE3:
			case eVertexType::UBYTE4:
				return GL_UNSIGNED_BYTE;

			case eVertexType::SHORT1:
			case eVertexType::SHORT2:
			case eVertexType::SHORT3:
			case eVertexType::SHORT4:
				return GL_SHORT;

			case eVertexType::USHORT1:
			case eVertexType::USHORT2:
			case eVertexType::USHORT3:
			case eVertexType::USHORT4:
				return GL_UNSIGNED_SHORT;

			case eVertexType::FLOAT1:
			case eVertexType::FLOAT2:
			case eVertexType::FLOAT3:
			case eVertexType::FLOAT4:
				return GL_FLOAT;
			}

			d_assert (0);

			return 0;
		}

		static int GetGLVertTypeCount(eVertexType type)
		{
			switch (type)
			{
			case eVertexType::BYTE1:
			case eVertexType::BYTE2:
			case eVertexType::BYTE3:
			case eVertexType::BYTE4:
				return type - eVertexType::BYTE1 + 1;

			case eVertexType::UBYTE1:
			case eVertexType::UBYTE2:
			case eVertexType::UBYTE3:
			case eVertexType::UBYTE4:
				return type - eVertexType::UBYTE1 + 1;

			case eVertexType::SHORT1:
			case eVertexType::SHORT2:
			case eVertexType::SHORT3:
			case eVertexType::SHORT4:
				return type - eVertexType::SHORT1 + 1;

			case eVertexType::USHORT1:
			case eVertexType::USHORT2:
			case eVertexType::USHORT3:
			case eVertexType::USHORT4:
				return type - eVertexType::USHORT1 + 1;

			case eVertexType::FLOAT1:
			case eVertexType::FLOAT2:
			case eVertexType::FLOAT3:
			case eVertexType::FLOAT4:
				return type - eVertexType::FLOAT1 + 1;
			}

			d_assert (0);

			return 0;
		}

		static int GetGLDrawCount(ePrimType type, int primCount)
		{
			switch (type)
			{
			case ePrimType::TRIANGLE_LIST:
				return primCount * 3;

			case ePrimType::TRIANGLE_STRIP:
				return primCount + 2;

			case ePrimType::LINE_LIST:
				return primCount * 2;

			case ePrimType::LINE_STRIP:
				return primCount + 1;

			case ePrimType::POINT_LIST:
				return primCount;
			}

			d_assert (0);

			return 0;
		}

		static GLenum GetGLUsage(eUsage usage)
		{
			if (usage == eUsage::DYNAMIC || usage == eUsage::DYNAMIC_MANAGED)
			{
				return GL_DYNAMIC_DRAW;
			}
			else
			{
				return GL_STATIC_DRAW;
			}
		}
	};


#ifdef M_PLATFORM_WIN32

	/*struct EglStruct
	{
	EGLDisplay  display;
	EGLConfig   config;
	EGLContext  context;
	EGLSurface  surface;
	};

	extern EglStruct gEGL;

	int EGL_CreateContext(HWND hWnd);
	void EGL_DestroyContext();

	void EGL_SwapBuffer();*/

	void WGL_CreateContext(HWND hWnd, const RenderSystem::Config & config);
	void WGL_DestroyContext();
	void WGL_SwapBuffers();

#endif

}
