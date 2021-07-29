#pragma once
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

namespace Log
{
	class Log;
	struct InternalLogInfo;

	enum class eLogWriter
	{
		Console,
		OutputDebug,
		File
	};

	class Writer
	{
	public:
		Writer();
		virtual ~Writer();		
		void PushMessage(InternalLogInfo* logInfo);
		void Release();

	protected:				
		virtual void Write(InternalLogInfo* logInfo) = 0;

	private:
		void Run();

		bool mIsRun;
		std::thread mThread;
		std::condition_variable mCV;
		std::mutex mLock;
		std::queue<InternalLogInfo*> mLogReadyQueue;
		std::queue<InternalLogInfo*> mLogDispatchQueue;
	};

/*
 * end of namespace
 */
}