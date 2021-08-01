#include "./Util/Time/Time.h"

namespace garam
{
	namespace time
	{
		LARGE_INTEGER Time::mFreq;
		LARGE_INTEGER Time::mCurrentTime;
		LARGE_INTEGER Time::mPrevTime;

		float Time::mDeltaTime;

		Time::Time()
		{
			Initialize();
		}

		Time::~Time()
		{
			timeEndPeriod(1);
		}

		void Time::Initialize()
		{
			timeBeginPeriod(1);
			QueryPerformanceFrequency(&mFreq);

			QueryPerformanceCounter(&mCurrentTime);
			QueryPerformanceCounter(&mPrevTime);
		}

		void Time::Update()
		{
			QueryPerformanceCounter(&mCurrentTime);

			mDeltaTime = (float)(mPrevTime.QuadPart - mCurrentTime.QuadPart);
			mPrevTime = mCurrentTime;
		}

		float Time::GetDeltaTime()
		{
			return (mDeltaTime / (float)mFreq.QuadPart) * -1;
		}

		int Time::GetDeltaTimeAtMillisecond()
		{
			return (int)(mDeltaTime / 10000.0f) * -1;
		}

	}
}