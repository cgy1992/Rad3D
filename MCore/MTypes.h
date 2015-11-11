/*
*	Types
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MObject.h"

#define MAX_HW_RENDERTARGET 4
#define MAX_HW_BONE 35
#define MAX_HW_UNIFORM 32
#define MAX_HW_SAMPLER 8
#define MAX_HW_VELEMENT 16
#define MAX_HW_USER_CONST 64
#define MAX_HW_SHADER_PASS 8
#define MAX_HW_VERTEX_STREAM 4
#define MAX_HW_TEXTURE_SIZE 4096

#define LIGHTING_COLOR_STREAM 3

#define MAIN_RENDER_CONTEXT_ID 2048
#define MAIN_RENDER_CONTEXT_ORDER 2048

namespace Rad {

	struct M_ENTRY eUsage
	{
		enum enum_t {
			SYSTEM,

			STATIC,
			STATIC_MANAGED,

			DYNAMIC,
			DYNAMIC_MANAGED,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eUsage);
	};

	struct M_ENTRY eLockFlag
	{
		enum enum_t {
			READ = 1,
			WRITE = 2,
			READ_WRITE = READ | WRITE,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eLockFlag);
	};

	//
	struct M_ENTRY eSamplerBindType
	{
		enum enum_t {
			NONE,
			EMISSIVE,
			DIFFUSE,
			SPECULAR,
			NORMAL,
			LIGHTING_MAP,

			EXTERN0,
			EXTERN1,
			EXTERN2,
			EXTERN3,
			EXTERN4,
			EXTERN5,
			EXTERN6,
			EXTERN7,

			SAMPLER,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eSamplerBindType);
	};

	//
	struct M_ENTRY ePrimType
	{
		enum enum_t {
			POINT_LIST,
			LINE_LIST,
			LINE_STRIP,
			TRIANGLE_LIST,
			TRIANGLE_STRIP,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(ePrimType);
	};

	//
	struct M_ENTRY eSPType
	{
		enum enum_t {
			FLOAT4,
			MATRIX4,

			WORLD_MATRIX,
			VIEW_MATRIX,
			PROJ_MATRIX,
			WORLD_VIEW_MATRIX,
			VIEW_PROJ_MATRIX,
			WORLD_VIEW_PROJ_MATRIX,
			BONE_MATRIX,

			CAMERA_POSITION,
			CAMERA_RIGHT_VECTOR,
			CAMERA_UP_VECTOR,
			CAMERA_DIR_VECTOR,

			LIGHT_POSITION,
			LIGHT_DIRECTION,
			LIGHT_ATTEN_PARAM,
			LIGHT_SPOT_PARAM,

			EMISSIVE,
			AMBIENT,
			DIFFUSE,
			SPECULAR,
			OPACITY,

			FOG_COLOR,
			FOG_PARAM,

			SHADER_STATE,

			TIME, // x - time, y - sin(t), z - cos(t), w - 1

			CLIP_PLANE, // x: near, y: far, z: 1 / near, w: 1 / far

			USER_CONST,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eSPType);
	};


	//
	struct M_ENTRY eTexAddress
	{
		enum enum_t {
			WRAP,
			MIRROR,
			CLAMP,
			BORDER,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eTexAddress);
	};


	struct M_ENTRY eTexFilter
	{
		enum enum_t {
			DEFAULT,
			POINT,
			LINEAR,
			ANISOTROPIC,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eTexFilter);
	};


	//
	struct M_ENTRY eVertexSemantic
	{
		enum enum_t {
			UNKNOWN = 0xFF,

			POSITION = 0,
			NORMAL,
			COLOR,
			LIGHTING_COLOR,

			BONE_INDEX,
			BONE_WEIGHT,

			TEXCOORD0,
			TEXCOORD1,
			TEXCOORD2,
			TEXCOORD3,

			MAX,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eVertexSemantic);
	};


	//
	struct M_ENTRY eVertexType
	{
		enum enum_t {
			UNKNOWN,
			BYTE1,
			BYTE2,
			BYTE3,
			BYTE4,
			UBYTE1,
			UBYTE2,
			UBYTE3,
			UBYTE4,
			SHORT1,
			SHORT2,
			SHORT3,
			SHORT4,
			USHORT1,
			USHORT2,
			USHORT3,
			USHORT4,
			FLOAT1,
			FLOAT2,
			FLOAT3,
			FLOAT4,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eVertexType);
	};

	//
	struct M_ENTRY ePixelFormat
	{
		enum enum_t {
			UNKNOWN,

			// Color format
			L8,
			L8A8,
			R8G8B8,
			R8G8B8A8,
			R5G6B5,
			R5G5B5A1,
			R4G4B4A4,

			// Float format
			R16F,
			R16G16F,
			R16G16B16A16F,

			R32F,
			R32G32F,
			R32G32B32A32F,

			// Compressed format
			DXT1_RGB,
			DXT3_RGBA,
			DXT5_RGBA,

			ETC1_RGB,
			//ETC2_RGB
			//ETC2_RGBA

			PVRTC1_RGB,
			//PVRTC2_RGB,
			//PVRTC2_RGBA,

			// DepthBuffer format
			D16,
			D24,
			D24S8,

			//
			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(ePixelFormat);
	};

	//
	struct M_ENTRY eCullMode
	{
		DECLARE_ENUM(eCullMode);

		enum enum_t {
			NONE,
			FRONT,
			BACK,

			INHERIT,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eCullMode);
	};

	//
	struct M_ENTRY eFillMode
	{
		DECLARE_ENUM(eFillMode);

		enum enum_t {
			POINT,
			FRAME,
			SOLID,

			INHERIT,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eFillMode);
	};

	//
	struct M_ENTRY eBlendMode
	{
		DECLARE_ENUM(eBlendMode);

		enum enum_t {
			OPACITY,		// target = source
			ALPHA_TEST,		//
			ALPHA_BLEND,	//
			ADD,			// target = srouce + current;
			MUL,			// target = source * current;
			COLOR_BLEND,	// target = source * source + (1 - source) * current;

			INHERIT,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eBlendMode);
	};

	//
	struct M_ENTRY eDepthMode
	{
		DECLARE_ENUM(eDepthMode);

		enum enum_t {
			NONE,
			ALWAYS,

			EQUAL,
			LESS,
			LESS_EQUAL,
			GREATER,
			GREATER_EQUAL,

			N_EQUAL,		// disable depth write
			N_LESS,
			N_LESS_EQUAL,
			N_GREATER,
			N_GREATER_EQUAL,

			INHERIT,		// value in material(not in shader)

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eDepthMode);
	};

	//
	struct M_ENTRY eColorMode
	{
		DECLARE_ENUM(eColorMode);

		enum enum_t {
			NONE = 0,
			RED = 1,
			GREEN = 2,
			BLUE = 4,
			ALPHA = 8,
			ALL = RED | GREEN | BLUE | ALPHA,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eColorMode);
	};

	//
	struct M_ENTRY eClearMode
	{
		enum enum_t {
			NONE = 0,
			COLOR = 1,
			DEPTH = 2,
			STENCIL = 4,
			ALL = COLOR | DEPTH | STENCIL,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eClearMode);
	};

	//
	struct M_ENTRY Viewport
	{
		int x, y;
		int w, h;

		Viewport() : x(0), y(0), w(0), h(0)
		{
		}

		Viewport(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h)
		{
		}
	};

	//
	struct M_ENTRY eLightType : public IEnum
	{
		DECLARE_ENUM(eLightType);

		enum enum_t {
			DIRECTION,
			POINT,
			SPOT,

			MAX,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eLightType);
	};

	//
	struct M_ENTRY eMapType
	{
		enum enum_t {
			EMISSIVE,
			DIFFUSE,
			SPECULAR,
			NORMAL,
			LIGHTING_MAP,

			EXTERN0,
			EXTERN1,
			EXTERN2,
			EXTERN3,
			EXTERN4,
			EXTERN5,
			EXTERN6,
			EXTERN7,

			MAX,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eMapType);
	};

	//
	struct M_ENTRY eStaticLightingMode : public IEnum
	{
		DECLARE_ENUM(eStaticLightingMode);

		enum enum_t {
			NONE,
			LIGHTING_COLOR,
			LIGHTING_MAP,

			FORCE_DWORD = 0x7FFFFFFF
		};

		M_ENUMERATION(eStaticLightingMode);
	};

	//
	struct eRenderCallBack {

		enum enum_t {
			SHADER = 0,

			UNUSED0 = 1,
			UNUSED1,
			UNUSED2,

			USER0 = 4,
			USER1,
			USER2,
			USER3,

			MAX,
		};

		M_ENUMERATION(eRenderCallBack)
	};

}