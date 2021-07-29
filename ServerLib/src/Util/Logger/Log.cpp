#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"

namespace garam
{
	namespace logger
	{
		std::wstring_view LogLevelToString(eLogLevel level)
		{
			switch (level)
			{
			case eLogLevel::Info:
				return L"Info";
			case eLogLevel::Error:
				return L"Error";
			}

			return L"";
		}

		Log::Log(Logger& logger, eLogLevel level)
			: mLogger(logger)
			, mLevel(level)
			, mStream(nullptr)
		{
			mStream = new std::wostringstream();
		}

		Log::~Log()
		{
			if (mStream != nullptr)
			{
				mLogger.Write(*this);
			}
		}
	}
}

