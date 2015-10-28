/*
*	Debug
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDefine.h"
#include <assert.h>

namespace Rad {

#if defined(M_DEBUG) || defined(M_PROFILE)
	#define d_assert(x) assert(x)
#else
	#define d_assert(x)
#endif

/*
    compile assert
        @example:
            compile_assert(sizeof(MyEnum) == sizeof(unsigned int));
*/
#define compile_assert(expn) typedef char __C_ASSERT__[(expn) ? 1 : -1]

/*
    array_count
        @example:
            int count = array_count(a);
*/
#define array_count(a) sizeof(a) / sizeof((a)[0])


/*
    while limit
*/
//#define while_limit(a, b) _while_limit(a, b)


/* :) float exception
-------------------------------------------------------------------
    @Remark:
        _EM_INVALID:        �Ƿ��쳣.
        _EM_ZERODIVIDE:     ��0�쳣.
        _EM_OVERFLOW:       ����쳣.
        _EM_INEXACT:        ����ȷ�쳣.(Ӧ�ùر�)
        _EM_UNDERFLOW:      ����. (Ӧ�ùر�)
        _EM_DENORMAL:       �������������쳣. (����ʱ��)
-------------------------------------------------------------------
*/
//#ifdef _DEBUG
//
//#pragma warning(push)
//#pragma warning(disable : 4996)
//
//    inline void __enable_fp_exception()
//    {
//        int i = _controlfp(0, 0);
//        i &= ~(EM_INVALID | EM_ZERODIVIDE | EM_OVERFLOW);
//        _controlfp(i, MCW_EM);
//    }
//
//    inline void __disable_fp_exception()
//    {
//        int i = _controlfp(0, 0);
//        i &= EM_INVALID | EM_ZERODIVIDE | EM_OVERFLOW;
//        _controlfp(i, MCW_EM);
//    }
//
//    #define _ENABLE_FP_EXCEPTION    __enable_fp_exception()
//    #define _DISABLE_FP_EXCEPTION   __disable_fp_exception()
//
//#pragma warning(pop)
//
//#else
//
//    #define _ENABLE_FP_EXCEPTION
//    #define _DISABLE_FP_EXCEPTION
//
//#endif
//    
//#define ENABLE_FP_EXCEPTION     _ENABLE_FP_EXCEPTION
//#define DISABLE_FP_EXCEPTION    _DISABLE_FP_EXCEPTION

}
