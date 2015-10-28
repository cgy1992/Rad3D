/*
*	Util
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MLog.h"
#include "MTypes.h"

namespace Rad { namespace util {

	inline eFillMode getFillMode(const String & str)
	{
		if (str == "INHERIT")
		{
			return eFillMode::INHERIT;
		}
		else if (str == "SOLID")
		{
			return eFillMode::SOLID;
		}
		else if (str == "FRAME")
		{
			return eFillMode::FRAME;
		}
		else if (str == "POINT")
		{
			return eFillMode::POINT;
		}
		else
		{
			d_log ("?: Unknown FillMode '%s'", str.c_str());
		}

		return eFillMode::SOLID;
	}

	inline eCullMode getCullMode(const String & str)
	{
		if (str == "INHERIT")
		{
			return eCullMode::INHERIT;
		}
		else if (str == "NONE")
		{
			return eCullMode::NONE;
		}
		else if (str == "FRONT")
		{
			return eCullMode::FRONT;
		}
		else if (str == "BACK")
		{
			return eCullMode::BACK;
		}
		else
		{
			d_log ("?: Unknown BlendMode '%s'", str.c_str());
		}

		return eCullMode::BACK;
	}

	inline eDepthMode getDepthMode(const String & str)
	{
		if (str == "INHERIT")
		{
			return eDepthMode::INHERIT;
		}
		else if (str == "NONE")
		{
			return eDepthMode::NONE;
		}
		else if (str == "ALWAYS")
		{
			return eDepthMode::ALWAYS;
		}
		else if (str == "EQUAL")
		{
			return eDepthMode::EQUAL;
		}
		else if (str == "LESS")
		{
			return eDepthMode::LESS;
		}
		else if (str == "LESS_EQUAL")
		{
			return eDepthMode::LESS_EQUAL;
		}
		else if (str == "GREATER")
		{
			return eDepthMode::GREATER;
		}
		else if (str == "GREATER_EQUAL")
		{
			return eDepthMode::GREATER_EQUAL;
		}
		else if (str == "N_EQUAL")
		{
			return eDepthMode::N_EQUAL;
		}
		else if (str == "N_LESS")
		{
			return eDepthMode::N_LESS;
		}
		else if (str == "N_LESS_EQUAL")
		{
			return eDepthMode::N_LESS_EQUAL;
		}
		else if (str == "N_GREATER")
		{
			return eDepthMode::N_GREATER;
		}
		else if (str == "N_GREATER_EQUAL")
		{
			return eDepthMode::N_GREATER_EQUAL;
		}
		else
		{
			d_log ("?: Unknown DepthMode '%s'", str.c_str());
		}

		return eDepthMode::LESS_EQUAL;
	}

	inline eBlendMode getBlendMode(const String & str)
	{
		if (str == "INHERIT")
		{
			return eBlendMode::INHERIT;
		}
		else if (str == "OPACITY")
		{
			return eBlendMode::OPACITY;
		}
		else if (str == "ALPHA_BLEND")
		{
			return eBlendMode::ALPHA_BLEND;
		}
		else if (str == "ADD")
		{
			return eBlendMode::ADD;
		}
		else if (str == "MUL")
		{
			return eBlendMode::MUL;
		}
		else if (str == "COLOR_BLEND")
		{
			return eBlendMode::COLOR_BLEND;
		}
		else
		{
			d_log ("?: Unknown BlendMode '%s'", str.c_str());
		}

		return eBlendMode::OPACITY;
	}

	inline eColorMode getColorMode(const String & str)
	{
		if (str == "ALL")
			return eColorMode::ALL;
		else if (str == "NONE")
			return eColorMode::NONE;
		else if (str == "RED")
			return eColorMode::RED;
		else if (str == "GREEN")
			return eColorMode::GREEN;
		else if (str == "BLUE")
			return eColorMode::BLUE;
		else if (str == "ALPHA")
			return eColorMode::ALPHA;
		else
		{
			d_log ("?: Unknown ColorMode '%s'.", str.c_str());
			return eColorMode::ALL;
		}
	}

	inline eVertexSemantic getVertexSemantic(const String & str)
	{
		if (str == "POSITION")
		{
			return eVertexSemantic::POSITION;
		}
		else if (str == "NORMAL")
		{
			return eVertexSemantic::NORMAL;
		}
		else if (str == "COLOR")
		{
			return eVertexSemantic::COLOR;
		}
		else if (str == "LIGHTING_COLOR")
		{
			return eVertexSemantic::LIGHTING_COLOR;
		}
		else if (str == "BONE_INDEX")
		{
			return eVertexSemantic::BONE_INDEX;
		}
		else if (str == "BONE_WEIGHT")
		{
			return eVertexSemantic::BONE_WEIGHT;
		}
		else if (str == "TEXCOORD0")
		{
			return eVertexSemantic::TEXCOORD0;
		}
		else if (str == "TEXCOORD1")
		{
			return eVertexSemantic::TEXCOORD1;
		}
		else if (str == "TEXCOORD2")
		{
			return eVertexSemantic::TEXCOORD2;
		}
		else if (str == "TEXCOORD3")
		{
			return eVertexSemantic::TEXCOORD3;
		}
		else
		{
			d_log ("?: Unknown Attribute '%s'", str.c_str());
			return eVertexSemantic::UNKNOWN;
		}
	}

	inline eSPType getSPType(const String & str)
	{
		if (str == "FLOAT4")
		{
			return eSPType::FLOAT4;
		}
		else if (str == "MATRIX4")
		{
			return eSPType::MATRIX4;
		}
		else if (str == "WORLD_MATRIX")
		{
			return eSPType::WORLD_MATRIX;
		}
		else if (str == "VIEW_MATRIX")
		{
			return eSPType::VIEW_MATRIX;
		}
		else if (str == "PROJ_MATRIX")
		{
			return eSPType::PROJ_MATRIX;
		}
		else if (str == "WORLD_VIEW_MATRIX")
		{
			return eSPType::WORLD_VIEW_MATRIX;
		}
		else if (str == "VIEW_PROJ_MATRIX")
		{
			return eSPType::VIEW_PROJ_MATRIX;
		}
		else if (str == "WORLD_VIEW_PROJ_MATRIX")
		{
			return eSPType::WORLD_VIEW_PROJ_MATRIX;
		}
		else if (str == "BONE_MATRIX")
		{
			return eSPType::BONE_MATRIX;
		}

		//
		else if (str == "CAMERA_POSITION")
		{
			return eSPType::CAMERA_POSITION;
		}
		else if (str == "CAMERA_DIR_VECTOR")
		{
			return eSPType::CAMERA_DIR_VECTOR;
		}
		else if (str == "CAMERA_UP_VECTOR")
		{
			return eSPType::CAMERA_UP_VECTOR;
		}
		else if (str == "CAMERA_RIGHT_VECTOR")
		{
			return eSPType::CAMERA_RIGHT_VECTOR;
		}

		//
		else if (str == "LIGHT_POSITION")
		{
			return eSPType::LIGHT_POSITION;
		}
		else if (str == "LIGHT_DIRECTION")
		{
			return eSPType::LIGHT_DIRECTION;
		}
		else if (str == "LIGHT_ATTEN_PARAM")
		{
			return eSPType::LIGHT_ATTEN_PARAM;
		}
		else if (str == "LIGHT_SPOT_PARAM")
		{
			return eSPType::LIGHT_SPOT_PARAM;
		}

		//
		else if (str == "EMISSIVE")
		{
			return eSPType::EMISSIVE;
		}
		else if (str == "AMBIENT")
		{
			return eSPType::AMBIENT;
		}
		else if (str == "DIFFUSE")
		{
			return eSPType::DIFFUSE;
		}
		else if (str == "SPECULAR")
		{
			return eSPType::SPECULAR;
		}
		else if (str == "OPACITY")
		{
			return eSPType::OPACITY;
		}

		//
		else if (str == "FOG_COLOR")
		{
			return eSPType::FOG_COLOR;
		}
		else if (str == "FOG_PARAM")
		{
			return eSPType::FOG_PARAM;
		}

		//
		else if (str == "SHADER_STATE")
		{
			return eSPType::SHADER_STATE;
		}

		//
		else if (str == "TIME")
		{
			return eSPType::TIME;
		}
		else if (str == "CLIP_PLANE")
		{
			return eSPType::CLIP_PLANE;
		}

		//
		else if (str == "USER_CONST")
		{
			return eSPType::USER_CONST;
		}
		else
		{
			d_log ("?: Unknown Uniform Type '%s'.", str.c_str());
			return eSPType::FLOAT4;
		}
	}

	inline eSamplerBindType getSamplerBindType(const String & str)
	{
		if (str == "NONE")
		{
			return eSamplerBindType::NONE;
		}
		else if (str == "EMISSIVE")
		{
			return eSamplerBindType::EMISSIVE;
		}
		else if (str == "DIFFUSE")
		{
			return eSamplerBindType::DIFFUSE;
		}
		else if (str == "NORMAL")
		{
			return eSamplerBindType::NORMAL;
		}
		else if (str == "SPECULAR")
		{
			return eSamplerBindType::SPECULAR;
		}
		else if (str == "LIGHTING_MAP")
		{
			return eSamplerBindType::LIGHTING_MAP;
		}
		else if (str == "EXTERN0")
		{
			return eSamplerBindType::EXTERN0;
		}
		else if (str == "EXTERN1")
		{
			return eSamplerBindType::EXTERN1;
		}
		else if (str == "EXTERN2")
		{
			return eSamplerBindType::EXTERN2;
		}
		else if (str == "EXTERN3")
		{
			return eSamplerBindType::EXTERN3;
		}
		else if (str == "EXTERN4")
		{
			return eSamplerBindType::EXTERN4;
		}
		else if (str == "EXTERN5")
		{
			return eSamplerBindType::EXTERN5;
		}
		else if (str == "EXTERN6")
		{
			return eSamplerBindType::EXTERN6;
		}
		else if (str == "EXTERN7")
		{
			return eSamplerBindType::EXTERN7;
		}
		else if (str == "SAMPLER")
		{
			return eSamplerBindType::SAMPLER;
		}
		else
		{
			d_log ("?: Unknown SamplerBindType '%s'.", str.c_str());
			return eSamplerBindType::DIFFUSE;
		}
	}

	inline eTexFilter getTexFilter(const String & str)
	{
		if (str == "DEFAULT")
		{
			return eTexFilter::DEFAULT;
		}
		else if (str == "POINT")
		{
			return eTexFilter::POINT;
		}
		else if (str == "LINEAR")
		{
			return eTexFilter::LINEAR;
		}
		else if (str == "ANISOTROPIC")
		{
			return eTexFilter::ANISOTROPIC;
		}
		else
		{
			d_log ("?: Unknown TexFilter '%s' In ShaderFX '%s'.", str.c_str());
			return eTexFilter::DEFAULT;
		}
	}

	inline eTexAddress getTexAddress(const String & str)
	{
		if (str == "WRAP")
		{
			return eTexAddress::WRAP;
		}
		else if (str == "MIRROR")
		{
			return eTexAddress::MIRROR;
		}
		else if (str == "CLAMP")
		{
			return eTexAddress::CLAMP;
		}
		else if (str == "BORDER")
		{
			return eTexAddress::BORDER;
		}
		else
		{
			d_log ("?: Unknown TexAddress '%s'.", str.c_str());
			return eTexAddress::WRAP;
		}
	}

	inline Float4 getBorderColor(const char * str)
	{
		float r = 0, g = 0, b = 0, a = 0;

		str = str_getfloat(r, str);
		str = str_getfloat(g, str);
		str = str_getfloat(b, str);
		str = str_getfloat(a, str);

		return Float4(r, g, b, a);
	}

	inline Float4 getUniformValue(const char * str)
	{
		float r = 0, g = 0, b = 0, a = 0;

		str = str_getfloat(r, str);
		str = str_getfloat(g, str);
		str = str_getfloat(b, str);
		str = str_getfloat(a, str);

		return Float4(r, g, b, a);
	}

	inline Float3 Float4ToFloat3(const Float4 & f4)
	{
		return Float3(f4.x, f4.y, f4.z);
	}

	inline Float4 Float3ToFloat4(const Float3 & f3)
	{
		return Float4(f3.x, f3.y, f3.z, 1);
	}

	inline Color TO_RGBE(const Float3 & color)
	{
		Color c;

		float e;
		e = Max(color.r, color.g);
		e = Max(e, color.b);

		if (e <= 1)
		{
			c.r = (unsigned char)(color.r * 255);
			c.g = (unsigned char)(color.g * 255);
			c.b = (unsigned char)(color.b * 255);
			c.a = 0;
		}
		else
		{
			Float3 rk = color / e;
			e = Min(e, 3.0f) - 1;

			c.r = (unsigned char)(rk.r * 255);
			c.g = (unsigned char)(rk.g * 255);
			c.b = (unsigned char)(rk.b * 255);
			c.a = (unsigned char)(e / 2.0f * 255);
		}

		return c;
	}

	inline Color TO_RGBE_EX(const Float4 & color)
	{
		return TO_RGBE(Float4ToFloat3(color));
	}

	inline Float3 RGBE_TO(Color color)
	{
		Float3 c;

		float e = 1 + color.a / 255.0f * 2.0f;
		c.r = color.r / 255.0f * e;
		c.g = color.g / 255.0f * e;
		c.b = color.b / 255.0f * e;

		return c;
	}

	inline Float4 RGBE_TO_EX(Color color)
	{
		Float4 c;

		float e = 1 + color.a / 255.0f * 2.0f;
		c.r = color.r / 255.0f * e;
		c.g = color.g / 255.0f * e;
		c.b = color.b / 255.0f * e;
		c.a = 1;

		return c;
	}

	inline bool LineIntersects(Float2 & cp, const Float2 & A, const Float2 & B, const Float2 & C, const Float2 & D)
	{
		float denominator = (B.x-A.x)*(D.y-C.y) - (B.y-A.y)*(D.x-C.x);

		if (Math::Equal(denominator, 0.0f))
			return false;

		float numeratorR = (A.y-C.y)*(D.x-C.x) - (A.x-C.x)*(D.y-C.y);
		float numeratorS = (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y);

		float r = numeratorR / denominator;
		float s = numeratorS / denominator;

		cp.x = A.x+r*(B.x-A.x);
		cp.y = A.y+r*(B.y-A.y);

		if ( r >= 0 && r <= 1 && s >= 0 && s <= 1 )
			return true;
		else
			return false;
	}

	inline void FastNormalize2(Float2 & n)
	{
		float len = n.LengthSq();
		if (len > EPSILON_E4)
		{
			n *= Math::FastInvSqrt(len);
		}
	}

	inline void FastNormalize3(Float3 & n)
	{
		float len = n.LengthSq();
		if (len > EPSILON_E4)
		{
			n *= Math::FastInvSqrt(len);
		}
	}

	inline Float2 RandomDir2()
	{
		float theta = Math::RandomUnit() * PI2;

		Float2 d;
		d.x = Math::FastCos(theta);
		d.y = Math::FastSin(theta);

		return d;
	}

	inline Float3 RandomDir3()
	{
		float theta = Math::RandomUnit() * PI;
		float beta = Math::RandomUnit() * PI2;

		float r = Math::FastSin(theta);

		Float3 d;
		d.x = Math::FastCos(beta) * r;
		d.y = Math::FastCos(theta);
		d.z = Math::FastSin(beta) * r;

		return d;
	}

}
}