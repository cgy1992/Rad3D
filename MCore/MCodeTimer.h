/*
*	CodeTimer
*
*	Author: SiZhong.Wang, M-001
*
*	CopyRight: SilverEyes Information CO. LTD.
*/
#pragma once

#include "MDefine.h"
#include "MSingleton.h"

#ifdef M_PROFILE

#define profile_code() profile_code_time(__LINE__)

#define profile_code_time(name) \
	static Rad::CodeTimer __ct_##name(__FUNCTION__); \
	CodeTimerFn _ct_func_##name(&__ct_##name)

#define begin_code_time(name)   \
	static Rad::CodeTimer(#name) __ct_##name; \
	__ct_##name.Begin()

#define end_code_time(name)     static __s_##name.End()

#define output_all_code_time()  Rad::CodeTimer::OuputAll()

#else

#define profile_code()
#define profile_code_time(name)

#define begin_code_time(name)
#define end_code_time(name)

#define output_all_code_time()

#endif

namespace Rad {

	class M_ENTRY CodeTimer
	{
	public:
		CodeTimer(const char * name);
		~CodeTimer();

		void
			Begin();
		void
			End();
		void            
			Reset();
		void            
			Output() const;
		static void 
			OuputAll();

		const char *    
			Function() const { return i_function; }
		float           
			AverageTime() const { return i_ave_time; }
		float           
			TotalTime() const { return i_total_time; }
		float          
			CurrentTime() const { return i_current_time; }
		uint32          
			TotalCalls() const { return i_total_calls; }
		float           
			MaxTime() const { return i_max_time; }
		float           
			MinTime() const { return i_min_time; }

	protected:
		char i_function[256];
		float i_start_time;
		float i_total_time;
		float i_current_time;
		dword i_total_calls;
		float i_max_time;
		float i_min_time;
		float i_ave_time;
		CodeTimer * i_next;
	};

	struct CodeTimerFn
	{
		CodeTimer * i_timer;
		
		CodeTimerFn(CodeTimer * timer)
			: i_timer(timer)
		{
			i_timer->Begin();
		}

		~CodeTimerFn()
		{
			i_timer->End();
		}
	};

}
