
/* :) Abs
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Abs(T x)
{
    return ::abs(x);
}

template<>
inline float Math::Abs(float x)
{
    int y = INT_BIT(x);
    y &= IEEE_FLOAT_SIGN_MARK;
    return  FLOAT_BIT(y);
}



/* :) Sign
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Sign(T x)
{
    if (x < 0)
        return (T)-1;
    else
        return (T)1;
}



/* :) Clamp
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Clamp(T x, T low, T hight)
{
    if (x < low)
        return low;

    if (x > hight)
        return hight;

    return x;
}

/* :)
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/
template<class T>
inline T Math::Equal(T a, T b)
{
	return a == b;
}

template<>
inline float Math::Equal(float a, float b)
{
	return (Math::Abs(a - b) < EPSILON_E4);
}

/* :) Sqrt
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::Lerp(const T & a, const T & b, float k)
{
	if (k < 0)
		return a;
	else if (k > 1)
		return b;
	else
		return a + (b - a) * k;
}


/* :) IsPower
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline bool Math::IsPower2(int x)
{
	return !(x & (x - 1)) && x > 0;
}


/* :) Sqrt
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Sqrt(float x)
{
#ifdef X86_ASM

    __asm
    {
        fld     x;
        fsqrt;
        fstp    x;
    }

    return x;

#else
    return ::sqrtf(x);
#endif
}

/* :) Sin
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Sin(float x)
{
#ifdef X86_ASM

    __asm
    {
        fld     x;
        fsin;
        fstp    x;
    }

    return x;

#else

    return ::sin(x);

#endif
}


/* :) Cos
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Cos(float x)
{
#ifdef X86_ASM

    __asm
    {
        fld     x;
        fcos;
        fstp    x;
    }

    return x;

#elif defined M_ARM_ASM

#else 

    return ::cos(x);

#endif 

}


/* :) SinCos
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline void Math::SinCos(float x, float & s, float & c)
{
#ifdef X86_ASM

    __asm
    {
        mov     ecx, c;
        mov     edx, s;
        fld     x;
        fsincos;
        fstp    [ecx];
        fstp    [edx];
    }

#else

    s = ::sinf(x);
    c = ::cosf(x);

#endif
}



/* :) Tan
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Tan(float x)
{
#ifdef X86_ASM

    __asm
    {
        fld     x;
        fsincos;
        fdiv;
        fstp    x;
    }

    return x;

#else

    return ::tanf(x);

#endif
}



/* :) Tan
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Cot(float x)
{
#ifdef X86_ASM

    __asm
    {
        fld     x;
        fsincos;
        fxch;
        fdiv;
        fstp    x;
    }

    return x;

#else

    return 1.0f / ::tanf(x);

#endif
}


/* :) ASin
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::ASin(float x)
{
	if (x > -1.0f)
	{
		if (x < 1.0f)
			return ::asinf(x);
		else
			return PI * 0.5f;
	}
	else
	{
		return -PI * 0.5f;
	}
}


/* :) ACos
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::ACos(float x)
{
	if (x > -1.0f)
	{
		if (x < 1.0f)
			return ::acosf(x);
		else
			return 0;
	}
	else
	{
		return PI;
	}
}


/* :) ATan
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::ATan(float x)
{
#ifdef X86_ASM

    __asm
    {
        fld     x;
        fpatan;
        fstp    x;
    }

    return x;

#else

    return ::atanf(x);

#endif
}


inline float Math::ATan2(float y, float x)
{
	return ::atan2f(y, x);
}

/* :) Sinh
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Sinh(float x)
{
    return ::sinhf(x);
}


/* :) Cosh
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Cosh(float x)
{
    return ::coshf(x);
}


/* :) Tanh
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Tanh(float x)
{
    return ::tanhf(x);
}


/* :) Round
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Round(float x)
{
	return ::floorf(x + 0.5f);
}

/* :) Fmod
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Fmod(float x, float y)
{
    return ::fmod(x, y);
}


/* :) Floor
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Floor(float x)
{
#if 0

    int n = IEEE_FLOAT_MANTISSA(x);

    int shift = 23 - IEEE_FLOAT_EXPONENT(x);
    int y = (IEEE_FLOAT_MANTISSA(x) | 0x00800000) >> shift;
    int sign = IEEE_FLOAT_SIGN(x);
    y = (y ^ sign) - sign;
    sign = IEEE_FLOAT_EXPONENT_SIGN(x);
    y &= ~sign;
    y -= IEEE_FLOAT_SIGN(x) && (IEEE_FLOAT_EXPONENT_SIGN(x) || (IEEE_FLOAT_MANTISSA(x) & ((1 << shift) - 1)));
    return (float)y;

#else

    return (float)::floorf(x);

#endif
}

/* :) Ceil
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Ceil(float x)
{
#if 0

    int shift = 23 - IEEE_FLOAT_EXPONENT(x);
    int y = (IEEE_FLOAT_MANTISSA(x) | 0x00800000) >> shift;
    int sign = IEEE_FLOAT_SIGN(x);
    y = (y ^ sign) - sign;
    sign = IEEE_FLOAT_EXPONENT_SIGN(x);
    y &= ~sign;
    y += !IEEE_FLOAT_SIGN(x) && (IEEE_FLOAT_EXPONENT_SIGN(x) || (IEEE_FLOAT_MANTISSA(x) & ((1 << shift) - 1)));
    return (float)y;

#else

    return ::ceilf(x);

#endif
}


/* :) Exp
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Exp(float x)
{
    return ::expf(x);
}


/* :) Log
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Log(float x)
{
    return ::logf(x);
}


/* :) Pow
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::Pow(float x, float y)
{
    return ::powf(x, y);
}


/* :) SRandom
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline void Math::SRandom(unsigned int seed)
{
	srand(seed);
}


/* :) Random
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline int Math::Random()
{
	return rand();
}


inline float Math::RandomUnit()
{
    return  (float)Random() / (float)RAND_MAX;
}


/* :) RandRange
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
template<class T>
inline T Math::RandRange(T a, T b)
{
    return a + (T)(Random() % (b - a + 1)); 
}

template<>
inline float Math::RandRange(float a, float b)
{
    return a + (b - a) * RandomUnit();
}


/* :) DegreeToRadian
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::DegreeToRadian(float degree)
{
    return degree * DEG_TO_RAD_FACTORY;
}


/* :) RadianToDegree
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::RadianToDegree(float rad)
{
    return rad * RAD_TO_DEG_FACTORY;
}


/* :) DegreeNormalize
------------------------------------------------------------------------------
    @Remark:
        normalize degree in [0, 360).
------------------------------------------------------------------------------
*/
inline float Math::DegreeNormalize(float x)
{
    if (x >= 360.0f || x < 0)
        x -= 360.0f * Floor(x / 360.0f);

    return x;
}


/* :) RadianNormalize
------------------------------------------------------------------------------
    @Remark:
        normalize radian in [0, 2PI).
------------------------------------------------------------------------------
*/
inline float Math::RadianNormalize(float x)
{
    if (x >=  2 * PI || x < 0)
        x -= 2 * PI* Floor(x / (2 * PI));

    return x;
}


/* :) ComputeVariance
------------------------------------------------------------------------------
    @Remark:
                        n
        var = sqrt ( (  E(xi - avg)^2 / (n - 1)  ) )     
                        1
------------------------------------------------------------------------------
*/
inline float Math:: ComputeVariance(float * Array, int iCount)
{
    float sum = 0.0f, avg = 0.0f, var = 0.0f;
    for (int i = 0; i < iCount; ++i)
    {
        sum += Array[i];
    }

    avg = sum / (float)iCount;

    for (int i = 0; i < iCount; ++i)
    {
        var += (Array[i] - avg) * (Array[i] - avg);
    }

    var /= iCount - 1;
    var = Sqrt(var);

    return var;
}

/* :) ComputeGaussian
------------------------------------------------------------------------------
    @Remark:
                                             -(x - miu)^2 / (2 * rou^2)
        G(x) = (1 / (sqrt(2PI) * rou)) * e ^ 
                                            -((x - miu)^2 + (y - miu)^2)) / (2 * rou^2)
        G(x, y) = (1 / (2PI * rou^2)) * e ^ 
------------------------------------------------------------------------------
*/
inline float Math::ComputeGaussian(float x, float mid, float var)
{
    static const float InvSqrt2PI = 1.0f / Sqrt(2 * PI);
    float dx = x - mid;

    var = 1.0f / var;
    float exp_param = -dx * dx * 2.0f * var * var;

    return InvSqrt2PI * var * Exp(exp_param);
}

inline float Math::ComputeGaussian(float x, float y, float mid_x, float mid_y, float var)
{
    static const float Inv2PI = (1/ PI) * 0.5f;
    float dx = x - mid_x;
    float dy = y - mid_y;

    var = 1.0f / var;
    var *= var;

    float exp_param = -(dx * dx + dy * dy) * 2.0f * var;

    return Inv2PI * var * Exp(exp_param);
}

/* :) FastSin
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::FastSin(float x)
{
    int Sign = INT_BIT(x) & IEEE_FLOAT_SIGN_BIT;
    x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);

	int i = (int)x;
	float k = x - i;

    i &= TRI_TABLE_MASK;

    x = SIN_TABLE[i] + k * (SIN_TABLE[i + 1] - SIN_TABLE[i]);
    
	i = INT_BIT(x) ^ Sign;
    
	return FLOAT_BIT(i);
}

inline float Math::FastSin2(float x)
{
	int Sign = INT_BIT(x) & IEEE_FLOAT_SIGN_BIT;
	x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);

	int i = (int)x;

	i &= TRI_TABLE_MASK;

	x = SIN_TABLE[i];

	i = INT_BIT(x) ^ Sign;

	return FLOAT_BIT(i);
}

/* :) FastCos
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::FastCos(float x)
{
    x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);
  
	int i = (int)x;
	float k = x - i;

	i &= TRI_TABLE_MASK;
   
	return COS_TABLE[i] + k * (COS_TABLE[i + 1] - COS_TABLE[i]);
}

inline float Math::FastCos2(float x)
{
	x = Math::Abs(x * TRI_TABLE_RADIAN_MUL);

	int i = (int)x;

	i &= TRI_TABLE_MASK;

	return COS_TABLE[i];
}

/* :) FastACos
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::FastACos(float x)
{
	if (x > -1.0f)
	{
		if (x < 1.0f)
		{
			x = (x + 1) / 2;
			x *= TRI_TABLE_SIZE;

			int i = (int)x;
			float k = x - i;

			return ACOS_TABLE[i] + k * (ACOS_TABLE[i + 1] - ACOS_TABLE[i]);
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return PI;
	}
}

/* :) FastInvSqrt
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline float Math::FastInvSqrt(float x)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = x * 0.5F;
	y  = x;
	i  = * (long *)&y;
	i  = 0x5f3759df - ( i >> 1 );
	y  = * (float *)&i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
	//y  = y * ( threehalfs - ( x2 * y * y ) );  // 2nd iteration, this can be removed

	return y;
}

/* :) Log2le 
------------------------------------------------------------------------------
------------------------------------------------------------------------------
*/
inline int Math::Log2(int x)
{
	int l = -1; 

	while (x >= 256)
	{
		l += 8; 
		x >>= 8;
	}

	return l + LOG_2[x]; 
}
