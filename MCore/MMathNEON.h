/*
*	Math NEON
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

#ifdef M_SIMD_NEON

#define SIMD_Cross3			NEON_Cross3
#define SIMD_Cross4			NEON_Cross4
#define SIMD_Lerp4			NEON_Lerp4
#define SIMD_Transform3		NEON_Transform3
#define SIMD_Transform4		NEON_Transform4
#define SIMD_TransformA3	NEON_TransformA3
#define SIMD_TransformN3	NEON_TransformN3
#define SIMD_TransformN4	NEON_TransformN4
#define SIMD_MatAdd			NEON_MatAdd
#define SIMD_MatSub			NEON_MatSub
#define SIMD_MatMul			NEON_MatMul
#define SIMD_MatScale		NEON_MatScale
#define SIMD_MatLerp		NEON_MatLerp
//#define SIMD_QuatMul		NEON_QuatMul

namespace Rad {

	inline void NEON_Cross3(Float3 * vOut, const Float3 * v1, const Float3 * v2)
	{
		/*
		*     x = y1 * z2 - z1 * y2
		*     y = z1 * x2 - x1 * z2
		*     z = x1 * y2 - y1 * x2
		*/
		asm volatile (
			// d0 = y1, z1
			// d1 = z1, x1
			"flds			s3, [%0]		\n" 
			"add			%0, %0, #4		\n"
			"vld1.32		{d0}, [%0]		\n" 
			"vmov.f32		s2, s1			\n"		

			// d2 = z2, x2
			// d3 = y2, z2
			"flds			s5, [%1]		\n"
			"add			%1, %1, #4		\n"
			"vld1.32		{d3}, [%1]		\n" 
			"vmov.f32		s4, s7			\n" 

			// d4[0] = y1 * z2 - z1 * y2
			// d4[1] = z1 * x2 - x1 * z2
			"vmul.f32		d4, d0, d2		\n"
			"vmls.f32		d4, d1, d3		\n"

			// d5[0] = x1 * y2 - y1 * x2
			"vmul.f32		d5, d3, d1[1]	\n"
			"vmul.f32		d5, d0, d2[1]	\n"

			//
			"vst1.32		d4, [%2]		\n"
			"add			%2,	%2, #8		\n"
			"fsts			s10, [%2]		\n"

			: "+r"(v1), "+r"(v2), "+r"(vOut):
			: "d0", "d1", "d2", "d3", "d4", "d5", "memory"
		);
	}

	inline void NEON_Cross4(Float4 * vOut, const Float4 * v1, const Float4 * v2)
	{
		NEON_Cross3((Float3 *)vOut, (Float3 *)v1, (Float3 *)v2);
		vOut->w  = 1;
	}

	inline void NEON_Lerp4(Float4 * vOut, const Float4 * v1, const Float4 * v2, float t)
	{
		asm volatile (
			// q1 = v1
			// q2 = v2
			"vld1.32		{d2, d3}, [%0]		\n" 
			"vld1.32		{d4, d5}, [%1]		\n" 

			// q2[0] = t
			"flds			s0, [%2]			\n" 

			// q[0] = v1 + (v2 - v1) * t
			"vsub.f32		q3, q1, q2			\n"
			"vmla.f32		q4, q3, d0[0]		\n"

			//
			"vst1.32		{d8, d9}, [%3]		\n"

			: "+r"(v1), "+r"(v2), "+r"(t), "=r"(vOut):
			: "q0", "q1", "q2", "q3", "q4", "memory"
		);
	}

	inline void NEON_Transform4(Float4 * vOut, const Float4 * v, const Mat4 * m)
	{
		asm volatile (
			"vld1.32		{d0, d1}, [%0]		\n"	// q = v

			"vld1.32		{d2, d3}, [%1]!		\n" // q1 = m[0]
			"vld1.32		{d4, d5}, [%1]!		\n" // q2 = m[1]
			"vld1.32		{d6, d7}, [%1]!		\n" // q3 = m[2]
			"vld1.32		{d8, d9}, [%1]		\n" // q4 = m[3]

			"vmul.f32		q5,	q1, d0[0]		\n"
			"vmla.f32		q5,	q2, d0[1]		\n"
			"vmla.f32		q5,	q3, d1[0]		\n"
			"vmla.f32		q5,	q4, d1[1]		\n"

			"vst1.32		{d10, d11}, [%2]	\n"
			:
			: "r"(v), "r"(m),"r"(vOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "memory"
		);
	}

	inline void NEON_Transform3(Float3 * vOut, const Float3 * v, const Mat4 * m)
	{
		float v4[4];

		asm volatile (
			"vld1.32		{d0, d1}, [%0]		\n"	// q = v

			"vld1.32		{d2, d3}, [%1]!		\n" // q1 = m[0]
			"vld1.32		{d4, d5}, [%1]!		\n" // q2 = m[1]
			"vld1.32		{d6, d7}, [%1]!		\n" // q3 = m[2]
			"vld1.32		{d8, d9}, [%1]		\n" // q4 = m[3]

			"vmul.f32		q5,	q1, d0[0]		\n"
			"vmla.f32		q5,	q2, d0[1]		\n"
			"vmla.f32		q5,	q3, d1[0]		\n"
			"vadd.f32		q0,	q5, q4			\n"

			"vst1.32		{d0, d1}, [%2]	\n"
			:
			: "r"(v), "r"(m),"r"(v4)
			: "q0", "q1", "q2", "q3", "q4", "q5", "memory"
		);

		float inv_w = 1 / v4[3];
		vOut->x = v4[0] * inv_w;
		vOut->y = v4[1] * inv_w;
		vOut->z = v4[2] * inv_w;
	}

	inline void NEON_TransformA3(Float3 * vOut, const Float3 * v, const Mat4 * m)
	{
		asm volatile (
			// q0 = v
			"vld1.32		d0, [%0]			\n"
			"add			%0, %0, #4			\n"
			"flds			s3, [%0]			\n" 

			"vld1.32		{d2, d3}, [%1]!		\n" // q1 = m[0]
			"vld1.32		{d4, d5}, [%1]!		\n" // q2 = m[1]
			"vld1.32		{d6, d7}, [%1]!		\n" // q3 = m[2]
			"vld1.32		{d8, d9}, [%1]		\n" // q4 = m[3]

			"vmul.f32		q5,	q1, d0[0]		\n"
			"vmla.f32		q5,	q2, d0[1]		\n"
			"vmla.f32		q5,	q3, d1[0]		\n"
			"vadd.f32		q0,	q5, q4			\n"

			"vst1.32		d0, [%2]			\n"
			"add			%2,	%2, #8			\n"
			"fsts			s2, [%2]			\n"
			:
			: "r"(v), "r"(m),"r"(vOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "memory"
		);
	}

	inline void NEON_TransformN3(Float3 * vOut, const Float3 * v, const Mat4 * m)
	{
		asm volatile (
			// q0 = v
			"vld1.32		d0, [%0]			\n"
			"add			%0, %0, #4			\n"
			"flds			s3, [%0]			\n" 

			"vld1.32		{d2, d3}, [%1]!		\n" // q1 = m[0]
			"vld1.32		{d4, d5}, [%1]!		\n" // q2 = m[1]
			"vld1.32		{d6, d7}, [%1]		\n" // q3 = m[2]

			"vmul.f32		q5,	q1, d0[0]		\n"
			"vmla.f32		q5,	q2, d0[1]		\n"
			"vmla.f32		q5,	q3, d1[0]		\n"

			"vst1.32		d10, [%2]		\n"
			"add			%2,	%2, #8		\n"
			"fsts			s22, [%2]		\n"
			:
			: "r"(v), "r"(m), "r"(vOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "memory"
			);
	}

	inline void NEON_TransformN4(Float4 * vOut, const Float4 * v, const Mat4 * m)
	{
		asm volatile (
			"vld1.32		{d0,d1}, [%0]		\n" // q0 = v

			"vld1.32		{d2, d3}, [%1]!		\n" // q1 = m[0]
			"vld1.32		{d4, d5}, [%1]!		\n" // q2 = m[1]
			"vld1.32		{d6, d7}, [%1]		\n" // q3 = m[2]

			"vmul.f32		q5,	q1, d0[0]		\n"
			"vmla.f32		q5,	q2, d0[1]		\n"
			"vmla.f32		q5,	q3, d1[0]		\n"

			"vst1.32		{d10, d11}, [%2]	\n"
			:
			: "r"(v), "r"(m),"r"(vOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "memory"
		);

		vOut->w = 1;
	}

	inline void NEON_MatAdd(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2)
	{
		asm volatile (
			"vld1.32		{d0, d1}, [%0]!			\n"
			"vld1.32		{d2, d3}, [%0]!			\n"
			"vst1.32		{d4, d5}, [%0]!			\n"
			"vst1.32		{d6, d7}, [%0]			\n"

			"vld1.32		{d8, d9}, [%1]!			\n"
			"vld1.32		{d10, d11}, [%1]!		\n"
			"vst1.32		{d12, d13}, [%1]!		\n"
			"vst1.32		{d14, d15}, [%0]		\n"

			"vadd.f32		q9, q0, q4				\n"
			"vadd.f32		q10, q1, q5				\n"
			"vadd.f32		q11, q2, q6				\n"
			"vadd.f32		q12, q3, q7				\n"

			"vst1.32		{d18, d19}, [%2]!		\n"
			"vst1.32		{d20, d21}, [%2]!		\n"
			"vst1.32		{d22, d23}, [%2]!		\n"
			"vst1.32		{d24, d25}, [%2]!		\n"
			:
			: "r"(m1), "r"(m2),"r"(mOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "memory"
		);
	}

	inline void NEON_MatSub(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2)
	{
		asm volatile (
			"vld1.32		{d0, d1}, [%0]!			\n"
			"vld1.32		{d2, d3}, [%0]!			\n"
			"vst1.32		{d4, d5}, [%0]!			\n"
			"vst1.32		{d6, d7}, [%0]			\n"

			"vld1.32		{d8, d9}, [%1]!			\n"
			"vld1.32		{d10, d11}, [%1]!		\n"
			"vst1.32		{d12, d13}, [%1]!		\n"
			"vst1.32		{d14, d15}, [%0]		\n"

			"vsub.f32		q9, q0, q4				\n"
			"vsub.f32		q10, q1, q5				\n"
			"vsub.f32		q11, q2, q6				\n"
			"vsub.f32		q12, q3, q7				\n"

			"vst1.32		{d18, d19}, [%2]!		\n"
			"vst1.32		{d20, d21}, [%2]!		\n"
			"vst1.32		{d22, d23}, [%2]!		\n"
			"vst1.32		{d24, d25}, [%2]!		\n"
			:
			: "r"(m1), "r"(m2),"r"(mOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "memory"
		);
	}

	inline void NEON_MatMul(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2)
	{
		asm volatile (
			"vld1.32		{d0, d1}, [%0]!		\n"
			"vld1.32		{d2, d3}, [%0]!		\n"
			"vld1.32		{d4, d5}, [%0]!		\n"
			"vld1.32		{d6, d7}, [%0]		\n"

			"vld1.32		{d8, d9}, [%1]!		\n"
			"vld1.32		{d10, d11}, [%1]!	\n"
			"vld1.32		{d12, d13}, [%1]!	\n"
			"vld1.32		{d14, d15}, [%1]	\n"

			"vmul.f32		q8,	q4, d0[0]		\n"
			"vmul.f32		q9,	q4, d2[0]		\n"
			"vmul.f32		q10,q4, d4[0]		\n"
			"vmul.f32		q11,q4, d6[0]		\n"

			"vmla.f32		q8,	q5, d0[1]		\n"
			"vmla.f32		q9,	q5, d2[1]		\n"
			"vmla.f32		q10,q5, d4[1]		\n"
			"vmla.f32		q11,q5, d6[1]		\n"

			"vmla.f32		q8,	q6, d1[0]		\n"
			"vmla.f32		q9,	q6, d3[0]		\n"
			"vmla.f32		q10,q6, d5[0]		\n"
			"vmla.f32		q11,q6, d7[0]		\n"

			"vmla.f32		q8,	q7, d1[1]		\n"
			"vmla.f32		q9,	q7, d3[1]		\n"
			"vmla.f32		q10,q7, d5[1]		\n"
			"vmla.f32		q11,q7, d7[1]		\n"

			"vst1.32		{d16, d17}, [%2]!	\n"
			"vst1.32		{d18, d19}, [%2]!	\n"
			"vst1.32		{d20, d21}, [%2]!	\n"
			"vst1.32		{d22, d23}, [%2]	\n"
			:
			: "r"(m1), "r"(m2),"r"(mOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "memory"
		);
	}

	inline void NEON_MatScale(Mat4 * mOut, const Mat4 * m1, float scale)
	{
		asm volatile (
			"flds			s0, [%1]				\n" 

			"vld1.32		{d2, d3}, [%0]!			\n"
			"vld1.32		{d4, d5}, [%0]!			\n"
			"vld1.32		{d6, d7}, [%0]!			\n"
			"vld1.32		{d8, d9}, [%0]			\n"

			"vmul.f32		q5,	q1, d0[0]			\n"
			"vmul.f32		q6,	q2, d0[0]			\n"
			"vmul.f32		q7,	q3, d0[0]			\n"
			"vmul.f32		q8,	q4, d0[0]			\n"

			"vst1.32		{d10, d11}, [%2]!			\n"
			"vst1.32		{d12, d13}, [%2]!			\n"
			"vst1.32		{d14, d15}, [%2]!			\n"
			"vst1.32		{d16, d17}, [%2]			\n"
			:
			: "r"(m1), "r"(scale),"r"(mOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "memory"
		);
	}

	inline void NEON_MatLerp(Mat4 * mOut, const Mat4 * m1, const Mat4 * m2, float t)
	{
		asm volatile (
			"vld1.32		{d2, d3}, [%0]!			\n"
			"vld1.32		{d4, d5}, [%0]!			\n"
			"vld1.32		{d6, d7}, [%0]!			\n"
			"vld1.32		{d8, d9}, [%0]			\n"

			"vld1.32		{d10, d11}, [%1]!		\n"
			"vld1.32		{d12, d13}, [%1]!		\n"
			"vld1.32		{d14, d15}, [%1]!		\n"
			"vld1.32		{d16, d17}, [%0]		\n"

			"vsub.f32		q10, q5, q1				\n"
			"vsub.f32		q11, q6, q2				\n"
			"vsub.f32		q12, q7, q3				\n"
			"vsub.f32		q6, q8, q4				\n"

			"flds			s0, [%2]				\n" 

			"vmla.f32		q1, q10, d[0]			\n"
			"vmla.f32		q2, q11, d[0]			\n"
			"vmla.f32		q3, q12, d[0]			\n"
			"vmla.f32		q4, q5, d[0]			\n"

			"vst1.32		{d2, d3}, [%3]!			\n"
			"vst1.32		{d4, d5}, [%3]!			\n"
			"vst1.32		{d6, d7}, [%3]!			\n"
			"vst1.32		{d8, d9}, [%3]			\n"
			:
			: "r"(m1), "r"(m2), "r"(t), "r"(mOut)
			: "q0", "q1", "q2", "q3", "q4", "q5", "q6", "q7", "q8", "q9", "q10", "q11", "q12", "memory"
		);
	}

}

#endif