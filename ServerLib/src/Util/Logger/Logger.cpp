#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"
#include "./Util/Logger/LogManager.h"

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
		/*
		 * ������ Console, File�� ������ϴ� �κ�
		 */
		for (auto iter = mWriters.begin();
			iter != mWriters.end();
			++iter)
		{
			(*iter)->Write(mName, log);
		}	
	}

// end of namespace
}


