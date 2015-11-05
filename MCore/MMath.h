/*
*	Math Lib
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDebug.h"
#include "MMemory.h"
#include "MFunctional.h"

namespace Rad {

	#define UNIT_METRES	100.0f

#ifndef PI
	#define PI			3.141592654f
#endif

#ifndef PI2
	#define PI2			(3.141592654f * 2)
#endif

#ifndef PI_HALF
	#define PI_HALF		(3.141592654f / 2)
#endif

	#define INV_255		(1.0f / 255.0f)

	#define SQRT_2		1.414213562f
	#define SQRT_3		1.732050808f

	#define EPSILON_E4 (float)(1E-4)
	#define EPSILON_E5 (float)(1E-5)
	#define EPSILON_E6 (float)(1E-6)
	#define DEFAULT_EPSILON EPSILON_E4

	#define MIN_SHORT	-32768
	#define MAX_SHORT	0x7FFF
	#define MIN_INT		-214748364
	#define MAX_INT		0x7FFFFFFF
	#define MIN_FLOAT	FLT_MIN
	#define MAX_FLOAT	FLT_MAX
	#define MAX_USHORT	0xFFFF
	#define MAX_UINT	0xFFFFFFFF


	#define IEEE_FLOAT_SIGN_BIT                 0x80000000
	#define IEEE_FLOAT_EXPONENT_BIT             0x7F800000
	#define IEEE_FLOAT_MANTISSA_BIT             0x007FFFFF
	#define IEEE_FLOAT_SIGN_MARK                0x7FFFFFFF
	#define IEEE_FLOAT_EXPONENT_MARK            0x807FFFFF
	#define IEEE_FLOAT_MANTISSA_MASK            0xFF800000
	#define IEEE_FLOAT_SIGN_POS                 31
	#define IEEE_FLOAT_MANTISSA_POS             23
	#define IEEE_FLOAT_STEP_BIT                 0x3F800000

	#define IEEE_FLOAT_SIGN(x)                  (INT_BIT(x) >> 31)
	#define IEEE_FLOAT_EXPONENT_SIGN(x)         (IEEE_FLOAT_EXPONENT(x) >> 31)
	#define IEEE_FLOAT_EXPONENT(x)              (((INT_BIT(x) & IEEE_FLOAT_EXPONENT_BIT) >> 23) - 127)
	#define IEEE_FLOAT_MANTISSA(x)              (INT_BIT(x) & IEEE_FLOAT_MANTISSA_BIT)

	#define IEEE_FLOAT_NOTSTANDARD(x)           ((x & 0x7F800000) == 0 && (x & 0x007FFFFF) != 0))
	#define IEEE_FLOAT_INFINTE(x)               ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) == 0))
	#define IEEE_FLOAT_NEGATIVE_INFINTE(x)      ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) == 0) && (x & 0x80000000) == 0x80000000))
	#define IEEE_FLOAT_POSITIVE_INFINTE(x)      ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) == 0) && (x & 0x80000000) == 0x00000000))
	#define IEEE_FLOAT_NAN(x)                   ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) != 0))
	#define IEEE_FLOAT_SNAN(x)                  ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) != 0) && (x & 0x00400000) == 0x00400000))
	#define IEEE_FLOAT_QNAN(x)                  ((x & 0x7F800000) == 0x7F800000 && (x & 0x007FFFFF) != 0) && (x & 0x00400000) == 0x00000000))

	#define INT_BIT(x)                          (*reinterpret_cast<int*>(&x))
	#define FLOAT_BIT(x)                        (*reinterpret_cast<float*>(&x))

	#define TRI_TABLE_SIZE                      2048
	#define TRI_TABLE_MASK                      (TRI_TABLE_SIZE - 1)
	#define TRI_TABLE_ANGLE_MUL                 ((float)TRI_TABLE_SIZE / 360.0f)
	#define TRI_TABLE_RADIAN_MUL                ((float)TRI_TABLE_SIZE / PI2)

	#define DEG_TO_RAD_FACTORY					(PI / 180.0f)
	#define RAD_TO_DEG_FACTORY					(180.0f / PI)



	//
	class M_ENTRY Math
	{
	public:
		static void
			Init();
		static void
			Shutdown();

		template<class T> static T
			Abs(T x);
		template<class T> static T
			Sign(T x);
		template<class T> static T
			Clamp(T x, T low, T hight);
		template<class T> static T
			Equal(T a, T b);

		template<class T> static T
			Lerp(const T & a, const T & b, float k);

		static bool
			IsPower2(int x);

		static float
			Sqrt(float x);
		static float
			Sin(float rads);
		static float
			Cos(float rads);
		static void
			SinCos(float rads, float & s, float & c);
		static float		
			Tan(float rads);
		static float		
			Cot(float rads);
		static float		
			ASin(float x);
		static float		
			ACos(float x);
		static float		
			ATan(float x);
		static float		
			ATan2(float y, float x);
		static float		
			Sinh(float x);
		static float		
			Cosh(float x);
		static float		
			Tanh(float x);     

		static float		
			Floor(float x);
		static float		
			Ceil(float x);
		static float		
			Round(float x);
		static float		
			Fmod(float x, float y);

		static float		
			Exp(float v);
		static float		
			Log(float v);
		static float		
			Pow(float base, float exponent);

		static void			
			SRandom(unsigned int seed);
		static int			
			Random();
		static float		
			RandomUnit();
		template<class T> static T
			RandRange(T a, T b);
		static float		
			PerlinNoise2(float x, float y, float scale);

		static float		
			ComputeVariance(float * array, int count);
		static float		
			ComputeGaussian(float x, float mid = 0.0f, float var = 1.0f);
		static float		
			ComputeGaussian(float x, float y, float mid_x = 0.0f, float mid_y = 0.0f, float var = 1.0f);

		static float		
			DegreeToRadian(float degree);
		static float		
			RadianToDegree(float rad);
		static float		
			DegreeNormalize(float x);
		static float		
			RadianNormalize(float x);

		static float		
			FastSin(float rads);
		static float		
			FastSin2(float rads);
		static float		
			FastCos(float rads);
		static float		
			FastCos2(float rads);
		static float		
			FastACos(float x);
		static float		
			FastInvSqrt(float x);

		static int			
			Log2(int x);

		static unsigned short
			Crc16(const char * str, int len = 0);
		static int
			Crc32(const char * str, int len = 0);
		static void
			Md5(char * o, const char * str, int len = 0);
		static void
			Md5_Init();
		static void
			Md5_Update(const char * str, int len = 0);
		static void
			Md5_Final(char * o);

		static float
			Noise2(float x, float y);

		static int
			RLE8_Encode(byte * dest, int dsize, const byte * src, int ssize);
		static int
			RLE8_Decode(byte * dest, int dsize, const byte * src, int ssize);

		static int
			RLE16_Encode(word * dest, int dsize, const word * src, int ssize);
		static int
			RLE16_Decode(word * dest, int dsize, const word * src, int ssize);

		static int
			RLE32_Encode(dword * dest, int dsize, const dword * src, int ssize);
		static int
			RLE32_Decode(dword * dest, int dsize, const dword * src, int ssize);

		static int // it's too slow
			LZ77_Encode(byte * dest, int dsize, const byte * src, int ssize);
		static int
			LZ77_Decode(byte * dest, int dsize, const byte * src, int ssize);

	public:
		static const float SIN_TABLE[TRI_TABLE_SIZE + 1];
		static const float COS_TABLE[TRI_TABLE_SIZE + 1];
		static const float ACOS_TABLE[TRI_TABLE_SIZE + 1];
		static const byte LOG_2[256];
	};

	struct IntFloatUnion
	{
		union {
			int v_int;
			float v_float;
		};
	};

#include "MMath.inl"

}

