/*
*	Math SSE
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MFloat3.h"
#include "MFloat4.h"
#include "MMat4.h"
#include "MQuat.h"
#include "MAabb.h"

#ifdef M_PLATFORM_WIN32
//#define M_SIMD_SSE
#endif

#ifdef M_SIMD_SSE

#define SIMD_Cross3			SSE_Cross3
#define SIMD_Cross4			SSE_Cross4
#define SIMD_Lerp4			SSE_Lerp4
#define SIMD_Transform3		SSE_Transform3
#define SIMD_Transform4		SSE_Transform4
#define SIMD_TransformA3	SSE_TransformA3
#define SIMD_TransformN3	SSE_TransformN3
#define SIMD_TransformN4	SSE_TransformN4
#define SIMD_MatAdd			SSE_MatAdd
#define SIMD_MatSub			SSE_MatSub
#define SIMD_MatMul			SSE_MatMul
#define SIMD_MatScale		SSE_MatScale
#define SIMD_MatLerp		SSE_MatLerp
#define SIMD_QuatMul		SSE_QuatMul

/*
code   symbol    decscrition
0       eq          == 
1       lt          <
2       le          <=
3       unord       un order 
4       ne          !=
5       ge          >=
6       gt          >
7       ord         order
*/
#define SSE_EQ          0
#define SSE_LT          1
#define SSE_LE          2
#define SSE_UNORD       3
#define SSE_NE          4
#define SSE_GE          5
#define SSE_GT          6
#define SSE_ORD         7
#define SSE_CMP_OK      0xFFFFFFFF
#define SSE_CMP_FAIL    0

#define SSE_ALIGN16 _declspec(align(16))
#define SSE_SHUFFLE(x, y, z, w)	((w & 3) << 6 | (z & 3) << 4 | (y & 3) << 2 | (x & 3))


namespace Rad {

	inline void SSE_Cross3(Float3 * vOut, const Float3 * v1, const Float3 * v2)
	{
		/*
		*     x = y1 * z2 - z1 * y2
		*     y = z1 * x2 - x1 * z2
		*     z = x1 * y2 - y1 * x2
		*/
		__asm
		{
			mov		esi,	v1;
			mov		edi,	v2;
			mov     edx,    vOut;
			movups	xmm0,	[esi];
			movups	xmm1,	[edi];
			movaps	xmm2,	xmm0;
			movaps	xmm3,	xmm1;
			shufps	xmm0,	xmm0,	11001001b;
			shufps	xmm1,	xmm1,	11010010b;
			mulps	xmm0,	xmm1;
			shufps	xmm2,	xmm2,	11010010b;
			shufps	xmm3,	xmm3,	11001001b;
			mulps	xmm2,	xmm3;
			subps	xmm0,	xmm2;

			movlps	[edx],	xmm0;
			shufps	xmm0,	xmm0,	SSE_SHUFFLE(2, 2, 2, 2);
			movss	8[edx], xmm0;
		}
	}

	inline void SSE_Cross4(Float4 * vOut, const Float4 * v1, const Float4 * v2)
	{
		/*
		*     x = y1 * z2 - z1 * y2
		*     y = z1 * x2 - x1 * z2
		*     z = x1 * y2 - y1 * x2
		*/
		__asm
		{
			mov		esi,	v1;
			mov		edi,	v2;
			mov     edx,    vOut;
			movups	xmm0,	[esi];
			movups	xmm1,	[edi];
			movaps	xmm2,	xmm0;
			movaps	xmm3,	xmm1;
			shufps	xmm0,	xmm0,	11001001b;
			shufps	xmm1,	xmm1,	11010010b;
			mulps	xmm0,	xmm1;
			shufps	xmm2,	xmm2,	11010010b;
			shufps	xmm3,	xmm3,	11001001b;
			mulps	xmm2,	xmm3;
			subps	xmm0,	xmm2;
			movups	[edx],	xmm0;
		}

		vOut->w = 1;
	}

#if 0
	// Return InvLength
	//
	inline float SSE_Normalize3(Float3 * vOut, const Float3 * v)
	{
		SSE_ALIGN16 Float4 v4;

		__asm
		{
			mov		esi,	v;
			lea     edx,    v4;

			movups  xmm0,   [esi];
			movaps  xmm1,   xmm0;
			mulps   xmm1,   xmm0;
			movaps  xmm3,   xmm1;
			movaps  xmm2,   xmm1;

			shufps  xmm3,   xmm3,   SSE_SHUFFLE(2, 2, 2, 2);
			shufps  xmm2,   xmm2,   SSE_SHUFFLE(1, 1, 1, 1);
			shufps  xmm1,   xmm1,   SSE_SHUFFLE(0, 0, 0, 0);

			addps   xmm2,   xmm3;
			addps   xmm1,   xmm2;

			rsqrtps xmm1,   xmm1;
			shufps  xmm1,   xmm1,   SSE_SHUFFLE(0, 0, 0, 0);
			mulps   xmm0,   xmm1;

			movaps	[edx],	xmm0;
			movss   [edx + 12], xmm1;
		}

		vOut->x = v4.x;
		vOut->y = v4.y;
		vOut->z = v4.z;

		return v4.w;
	}

	// Return InvLength
	//
	inline float SSE_Normalize4(Float4 * vOut, const Float4 * v)
	{
		SSE_ALIGN16 Float4 v4;

		__asm
		{
			mov		esi,	v;
			lea     edx,    v4;

			movups  xmm0,   [esi];
			movaps  xmm1,   xmm0;
			mulps   xmm1,   xmm0;

			movaps  xmm3,   xmm1;
			movaps  xmm2,   xmm1;

			shufps  xmm3,   xmm3,   SSE_SHUFFLE(2, 2, 2, 2);
			shufps  xmm2,   xmm2,   SSE_SHUFFLE(1, 1, 1, 1);
			shufps  xmm1,   xmm1,   SSE_SHUFFLE(0, 0, 0, 0);

			addps   xmm2,   xmm3;
			addps   xmm1,   xmm2;

			rsqrtps xmm1,   xmm1;
			shufps  xmm1,   xmm1,   SSE_SHUFFLE(0, 0, 0, 0);
			mulps   xmm0,   xmm1;

			movaps	[edx],	xmm0;
			movss   [edx + 12], xmm1;
		}

		vOut->x = v4.x;
		vOut->y = v4.y;
		vOut->z = v4.z;
		vOut->w = 1;

		return v4.w;
	}
#endif

	inline void SSE_TransformN3(Float3 * vOut, const Float3 * v, const Mat4 * mat)
	{
		__asm
		{
			mov		esi,	v;
			mov		edi,	mat;
			mov     edx,    vOut;

			movups	xmm4,	[edi];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];

			movss	xmm0,	[esi];
			movss	xmm1,	[esi + 4];
			movss	xmm2,	[esi + 8];

			shufps	xmm0,	xmm0,   0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;

			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;

			addps	xmm0,	xmm1;
			addps	xmm0,	xmm2;

			movlps	[edx],	xmm0;
			shufps	xmm0,	xmm0,	SSE_SHUFFLE(2, 2, 2, 2);
			movss	[edx + 8], xmm0;
		}
	}

	inline void SSE_TransformN4(Float4 * vOut, const Float4 * v, const Mat4 * mat)
	{
		__asm
		{
			mov		esi,	v;
			mov		edi,	mat;
			mov     edx,    vOut;

			movups	xmm4,	[edi];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];

			movss	xmm0,	[esi];
			movss	xmm1,	[esi + 4];
			movss	xmm2,	[esi + 8];

			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;

			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;

			addps	xmm0,	xmm1;
			addps	xmm0,	xmm2;

			movups	[edx],  xmm0;
		}

		vOut->w = 1;
	}

	inline void SSE_Transform3(Float3 * vOut, const Float3 * v, const Mat4 * mat)
	{
		__asm
		{
			mov		esi,	v;
			mov		edi,	mat;
			mov     edx,    vOut;

			movss	xmm0,	[esi];
			movss	xmm1,	[esi + 4];
			movss	xmm2,	[esi + 8];

			movups	xmm4,	[edi];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];
			movups	xmm7,	[edi + 48];

			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;

			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;

			addps	xmm0,	xmm1;
			addps	xmm2,	xmm7;
			addps	xmm0,	xmm2;

			//v.xyz = v.xyz / v.w
			movaps  xmm1,   xmm0;
			shufps  xmm1,   xmm1,   SSE_SHUFFLE(3, 3, 3, 3);

			divps   xmm0,   xmm1;

			//copy xy
			movlps	[edx],	xmm0;
			//copy z
			shufps	xmm0,	xmm0,	SSE_SHUFFLE(2, 2, 2, 2);
			movss	[edx + 8], xmm0;
		}
	}

	inline void SSE_Transform4(Float4 * vOut, const Float4 * v, const Mat4 * mat)
	{
		__asm
		{
			mov		esi,	v;
			mov		edi,	mat;
			mov     edx,    vOut;

			movups	xmm4,	[edi];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];
			movups	xmm7,	[edi + 48];

			movss	xmm0,	[esi];
			movss	xmm1,	[esi + 4];
			movss	xmm2,	[esi + 8];
			movss	xmm3,	[esi + 12];

			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;
			shufps	xmm3,	xmm3,	0;

			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;
			mulps	xmm3,	xmm7;

			addps	xmm0,	xmm1;
			addps	xmm2,	xmm3;
			addps	xmm0,	xmm2;

			movups	[edx], xmm0;
		}
	}

	inline void SSE_TransformA3(Float3 * vOut, const Float3 * v, const Mat4 * mat)
	{
		__asm
		{
			mov		esi,	v;
			mov		edi,	mat;
			mov     edx,    vOut;

			movss	xmm0,	[esi];
			movss	xmm1,	[esi + 4];
			movss	xmm2,	[esi + 8];

			movups	xmm4,	[edi];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];
			movups	xmm7,	[edi + 48];

			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;

			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;

			addps	xmm0,	xmm1;
			addps	xmm2,	xmm7;
			addps	xmm0,	xmm2;

			//copy xy
			movlps	[edx],	xmm0;
			//copy z
			shufps	xmm0,	xmm0,	SSE_SHUFFLE(2, 2, 2, 2);
			movss	[edx + 8], xmm0;
		}
	}

	inline void SSE_Lerp4(Float4 * vOut, const Float4 * v1, const Float4 * v2, float t)
	{
		__asm 
		{
			mov		esi,	v1;
			mov		edi,	v2;
			mov     edx,    vOut;

			movups  xmm0,   [esi];
			movups  xmm1,   [edi];
			movss   xmm2,   t;
			subps   xmm1,   xmm0;
			shufps  xmm2,   xmm2,   0;
			mulps   xmm1,   xmm2;
			addps   xmm0,   xmm1;

			movups [edx],	xmm0;
		}
	}

	inline void SSE_MatAdd(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2)
	{
		__asm
		{
			mov		esi,	m1;
			mov		edi,	m2;
			mov		edx,	mOut;

			movups	xmm0,	[esi + 0];
			movups	xmm1,	[esi + 16];
			movups	xmm2,	[esi + 32];
			movups	xmm3,	[esi + 48];

			movups	xmm4,	[edi + 0];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];
			movups	xmm7,	[edi + 48];

			addps	xmm0,	xmm4;
			addps	xmm1,	xmm5;
			addps	xmm2,	xmm6;
			addps	xmm3,	xmm7;

			movups  [edx],  xmm0;
			movups  16[edx], xmm1;
			movups  32[edx], xmm2;
			movups  48[edx], xmm3;
		}
	}

	inline void SSE_MatSub(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2)
	{
		__asm
		{
			mov		esi,	m1;
			mov		edi,	m2;
			mov		edx,	mOut;

			movups	xmm0,	[esi + 0];
			movups	xmm1,	[esi + 16];
			movups	xmm2,	[esi + 32];
			movups	xmm3,	[esi + 48];

			movups	xmm4,	[edi + 0];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];
			movups	xmm7,	[edi + 48];

			subps	xmm0,	xmm4;
			subps	xmm1,	xmm5;
			subps	xmm2,	xmm6;
			subps	xmm3,	xmm7;

			movups  [edx],  xmm0;
			movups  16[edx], xmm1;
			movups  32[edx], xmm2;
			movups  48[edx], xmm3;
		}
	}

	inline void SSE_MatMul(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2)
	{
		__asm
		{
			mov		esi,	m1;
			mov		edi,	m2;
			mov		edx,	mOut;
			movups	xmm4,	[edi];
			movups	xmm5,	[edi + 16];
			movups	xmm6,	[edi + 32];
			movups	xmm7,	[edi + 48];

			movss	xmm0,	[esi];
			movss	xmm1,	[esi + 4];
			movss	xmm2,	[esi + 8];
			movss	xmm3,	[esi + 12];
			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;
			shufps	xmm3,	xmm3,	0;
			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;
			mulps	xmm3,	xmm7;
			addps	xmm0,	xmm1;
			addps	xmm2,	xmm3;
			addps	xmm0,	xmm2;
			movups	[edx],	xmm0;

			movss	xmm0,	[esi + 16];
			movss	xmm1,	[esi + 20];
			movss	xmm2,	[esi + 24];
			movss	xmm3,	[esi + 28];
			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;
			shufps	xmm3,	xmm3,	0;
			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;
			mulps	xmm3,	xmm7;
			addps	xmm0,	xmm1;
			addps	xmm2,	xmm3;
			addps	xmm0,	xmm2;
			movups	[edx + 16], xmm0;

			movss	xmm0,	[esi + 32];
			movss	xmm1,	[esi + 36];
			movss	xmm2,	[esi + 40];
			movss	xmm3,	[esi + 44];
			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;
			shufps	xmm3,	xmm3,	0;
			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;
			mulps	xmm3,	xmm7;
			addps	xmm0,	xmm1;
			addps	xmm2,	xmm3;
			addps	xmm0,	xmm2;
			movups	[edx + 32], xmm0;

			movss	xmm0,	[esi + 48];
			movss	xmm1,	[esi + 52];
			movss	xmm2,	[esi + 56];
			movss	xmm3,	[esi + 60];
			shufps	xmm0,	xmm0,	0;
			shufps	xmm1,	xmm1,	0;
			shufps	xmm2,	xmm2,	0;
			shufps	xmm3,	xmm3,	0;
			mulps	xmm0,	xmm4;
			mulps	xmm1,	xmm5;
			mulps	xmm2,	xmm6;
			mulps	xmm3,	xmm7;
			addps	xmm0,	xmm1;
			addps	xmm2,	xmm3;
			addps	xmm0,	xmm2;
			movups	[edx + 48], xmm0;
		}
	}

	inline void SSE_MatScale(Mat4 * mOut, const Mat4 * m1, float scale)
	{
		__asm
		{
			mov     esi,    m1;
			mov     edi,    mOut;

			movss   xmm4,   scale;
			shufps  xmm4,   xmm4, SSE_SHUFFLE(0, 0, 0, 0);
			movups  xmm0,   [esi];
			movups  xmm1,   16[esi];
			movups  xmm2,   32[esi];
			movups  xmm3,   48[esi];

			mulps   xmm0,   xmm4;
			mulps   xmm1,   xmm4;
			mulps   xmm2,   xmm4;
			mulps   xmm3,   xmm4;

			movups  [edi],  xmm0;
			movups  16[edi], xmm1;
			movups  32[edi], xmm2;
			movups  48[edi], xmm3;
		}
	}

	inline void SSE_MatLerp(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2, float t)
	{
		__asm
		{
			mov		esi,	m1;
			mov		edi,	m2;
			mov     edx,    mOut;

			movss   xmm4,   t;
			shufps  xmm4,   xmm4,   0;

			movups  xmm0,   [esi];
			movups  xmm1,   16[esi];
			movups  xmm2,   [edi];
			movups  xmm3,   16[edi];

			subps   xmm2,   xmm0;
			subps   xmm3,   xmm1;

			mulps   xmm2,   xmm4;
			mulps   xmm3,   xmm4;

			addps   xmm0,   xmm2;
			addps   xmm1,   xmm3;

			movups  [edx],  xmm0;
			movups  16[edx],  xmm1;

			movups  xmm0,   32[esi];
			movups  xmm1,   48[esi];
			movups  xmm2,   32[edi];
			movups  xmm3,   48[edi];

			subps   xmm2,   xmm0;
			subps   xmm3,   xmm1;

			mulps   xmm2,   xmm4;
			mulps   xmm3,   xmm4;

			addps   xmm0,   xmm2;
			addps   xmm1,   xmm3;

			movups  32[edx],  xmm0;
			movups  48[edx],  xmm1;
		}
	}

	inline void SSE_QuatMul(Quat * quat, const Quat * q1, const Quat * q2)
	{
		static SSE_ALIGN16 Quat t1(1.0f, -1.0f, 1.0f, -1.0f);
		static SSE_ALIGN16 Quat t2(1.0f, 1.0f, -1.0f, -1.0f);
		static SSE_ALIGN16 Quat t3(-1.0f, 1.0f, 1.0f, -1.0f);

		__asm
		{
			mov esi, q1;
			mov edx, q2;
			mov edi, quat;

			movss   xmm0, 12[esi];      //w
			movss   xmm1, [esi];        //x
			movss   xmm2, 4[esi];       //y
			movss   xmm3, 8[esi];       //z

			movups  xmm4, [edx];
			movaps  xmm5, xmm4;
			movaps  xmm6, xmm4;
			movaps  xmm7, xmm4;

			shufps  xmm0, xmm0, SSE_SHUFFLE(0, 0, 0, 0);
			shufps  xmm1, xmm1, SSE_SHUFFLE(0, 0, 0, 0);
			shufps  xmm2, xmm2, SSE_SHUFFLE(0, 0, 0, 0);
			shufps  xmm3, xmm3, SSE_SHUFFLE(0, 0, 0, 0);

			shufps  xmm4, xmm4, SSE_SHUFFLE(0, 1, 2, 3);
			shufps  xmm5, xmm5, SSE_SHUFFLE(3, 2, 1, 0);
			shufps  xmm6, xmm6, SSE_SHUFFLE(2, 3, 0, 1);
			shufps  xmm7, xmm7, SSE_SHUFFLE(1, 0, 3, 2);

			mulps   xmm0, xmm4;
			mulps   xmm1, xmm5;
			mulps   xmm2, xmm6;
			mulps   xmm3, xmm7;

			movaps  xmm4, t1;
			movaps  xmm5, t2;
			movaps  xmm6, t3;

			mulps   xmm1, xmm4;  
			mulps   xmm2, xmm5;  
			mulps   xmm3, xmm6;  

			addps   xmm0, xmm1;
			addps   xmm2, xmm3;
			addps   xmm0, xmm2;

			movups [edi], xmm0; 
		}
	}

}

#endif