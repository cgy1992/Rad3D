#include "MCodeTimer.h"
#include "MRoot.h"

namespace Rad {

	CodeTimer * i_timer_linker = NULL;

	CodeTimer::CodeTimer(const char * func)
		: i_start_time(0)
		, i_total_time(0),
		i_current_time(0),
		i_total_calls(0),
		i_max_time(0),
		i_min_time(FLT_MAX),
		i_ave_time(0),
		i_next(NULL)
	{
		strcpy(i_function, func);

		this->i_next = i_timer_linker;
		i_timer_linker = this;
	}

	CodeTimer::~CodeTimer()
	{
	}

	void CodeTimer::Begin()
	{
		if (!i_start_time)
		{
			++i_total_calls;
			i_start_time = Root::Instance()->GetTime();
		}
	}

	void CodeTimer::End()
	{
		if (i_start_time)
		{
			i_current_time = Root::Instance()->GetTime() - i_start_time;
			assert (i_current_time >= 0);
			i_total_time +=  i_current_time;
			i_ave_time = (i_ave_time + i_current_time) * 0.5f;

			if (i_min_time > i_current_time)
				i_min_time = i_current_time;
			if (i_max_time < i_current_time)
				i_max_time = i_current_time;

			i_start_time = 0.0f;
		}
	}

	void CodeTimer::Reset()
	{
		i_start_time = 0.0f;
		i_total_time = 0.0f;
		i_total_calls = 0;
		i_max_time = 0.0f;
		i_min_time = FLT_MAX;
		i_ave_time = 0.0f;
	}

	void CodeTimer::Output() const
	{
		d_log(
			"  function:          %s\n" \
			"  total time:       %f\n" \
			"  current time:     %f\n" \
			"  average time:     %f\n" \
			"  total calls:      %d\n" \
			"  max time:         %f\n" \
			"  min time:         %f\n\n",
			Function(),
			TotalTime(),
			CurrentTime(),
			AverageTime(),
			TotalCalls(),
			MaxTime(),
			MinTime());
	}

	void CodeTimer::OuputAll()
	{
		d_log("\n****** Code Timer ******\n");

		CodeTimer * timer = i_timer_linker;

		while (timer)
		{
			timer->Output();
			timer = timer->i_next;
		}
	}

}
