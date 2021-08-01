#pragma once
#include <Windows.h>

namespace garam
{
	namespace time
	{
		class Time
		{
		public:
			Time();
			~Time();

			static void Initialize();
			static void Update();
			static float GetDeltaTime();
			static int GetDeltaTimeAtMillisecond();

		private:
			static LARGE_INTEGER mFreq;
			static LARGE_INTEGER mCurrentTime;
			static LARGE_INTEGER mPrevTime;

			static float mDeltaTime;
		};
	}
}