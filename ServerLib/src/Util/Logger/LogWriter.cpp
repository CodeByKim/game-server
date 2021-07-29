#include "./Util/Logger/LogWriter.h"
#include "./Util/Logger/Logger.h"

namespace garam
{
	namespace logger
	{
		Writer::Writer()
			: mIsRun(true)
			, mThread(std::thread(&Writer::Run, this))
		{
		}

		Writer::~Writer()
		{

		}

		void Writer::PushMessage(InternalLogInfo* logInfo)
		{
			mLock.lock();
			mLogReadyQueue.push(logInfo);
			mLock.unlock();

			mCV.notify_one();
		}

		void Writer::Release()
		{
			mIsRun = false;
			mCV.notify_one();

			mThread.join();
		}

		void Writer::Run()
		{
			while (mIsRun)
			{
				if (mLogDispatchQueue.empty())
				{
					std::unique_lock<std::mutex> lock(mLock);
					mCV.wait(lock, [&]() {
						return !mLogReadyQueue.empty() || !mIsRun;
						});

					if (!mIsRun)
					{
						lock.unlock();
						return;
					}

					std::swap(mLogReadyQueue, mLogDispatchQueue);
				}

				InternalLogInfo* logInfo = mLogDispatchQueue.front();
				mLogDispatchQueue.pop();

				Write(logInfo);

				logInfo->refCount -= 1;
				if (logInfo->refCount.load() == 0)
				{
					delete logInfo;
				}
			}
		}
	}	
}