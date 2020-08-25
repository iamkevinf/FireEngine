#ifndef __TIME_H__
#define __TIME_H__

#include "math/math.h"

namespace FireEngine
{
	class Time
	{
	public:
		static float GetTime();
		static long long GetTimeMS();

		static float GetDeltaTime() { return s_time_delta; }
		static uint32_t GetFrameCount() { return s_frame_count; }

		static uint32_t GetFPS() { return s_fps; }

		static void Tick();

	private:
		static long long s_time_startup;
		static float s_time_delta;
		static float s_time_record;
		static uint32_t s_frame_count;
		static uint32_t s_frame_record;
		static float s_time;
		static uint32_t s_fps;
	};
}

#endif // __TIME_H__