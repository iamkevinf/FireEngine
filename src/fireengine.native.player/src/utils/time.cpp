#include "time.h"

#include <time.h>

#include <bx/timer.h>

namespace FireEngine
{
	long long Time::s_time_startup = 0;
	float Time::s_time_delta = 0;
	float Time::s_time_record = -1;
	uint32_t Time::s_frame_count = 0;
	uint32_t Time::s_frame_record;
	float Time::s_time = 0;
	uint32_t Time::s_fps;

	long long Time::GetTimeMS()
	{
		long long t = bx::getHPCounter();

		return t;
	}

	float Time::GetTime()
	{
		if (s_time_startup == 0)
			s_time_startup = GetTimeMS();

		long long time = GetTimeMS() - s_time_startup;

		return time / 1000.0f;
	}

	void Time::Tick()
	{
		float time = Time::GetTime();
		Time::s_time_delta = time - Time::s_time;
		Time::s_time = time;

		if (Time::s_time_record < 0)
		{
			Time::s_time_record = Time::GetTime();
			Time::s_frame_record = Time::GetFrameCount();
		}

		uint32_t frame = Time::GetFrameCount();
		if (time - Time::s_time_record >= 1)
		{
			Time::s_fps = frame - Time::s_frame_record;
			Time::s_time_record = time;
			Time::s_frame_record = frame;
		}

		Time::s_frame_count++;
	}
}