#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"
#include "./Util/Logger/LogManager.h"

namespace garam
{
	namespace logger
	{
		Logger::Logger(Configure* config)
		{
			mName = config->mLoggerName;
		}

		Logger::~Logger()
		{

		}

		Log Logger::Info()
		{
			return Log(*this, eLogLevel::Info);
		}

		Log Logger::Error()
		{
			return Log(*this, eLogLevel::Error);
		}

		void Logger::SetWriter(Writer* writer)
		{
			mWriters.push_back(writer);
		}

		void Logger::Write(Log& log)
		{
			InternalLogInfo* logInfo = new InternalLogInfo(mName, log, (int)mWriters.size());

			for (auto iter = mWriters.begin();
				iter != mWriters.end();
				++iter)
			{
				(*iter)->PushMessage(logInfo);
			}
		}

		InternalLogInfo::InternalLogInfo(std::wstring_view loggerName, Log& log, int writerCount)
			: loggerName(loggerName)
			, level(log.mLevel)
			, stream(log.mStream)
			, refCount(writerCount)
		{
		}

		InternalLogInfo::~InternalLogInfo()
		{
			delete stream;
		}
	}	
}


