#pragma once
#include <string_view>
#include "Log.h"
#include "LogConfigure.h"

namespace garam
{
	namespace logger
	{
		class Wrtier;

		struct InternalLogInfo
		{
			InternalLogInfo(std::wstring_view loggerName, Log& log, int writerCount);
			~InternalLogInfo();

			std::wstring_view loggerName;
			eLogLevel level;
			std::wostringstream* stream;
			std::atomic_int refCount;
		};

		class Logger
		{
		public:
			Logger(Configure* config);
			~Logger();

			Log Info();
			Log Error();

			void SetWriter(Writer* writer);
			void Write(Log& log);

		private:
			std::vector<Writer*> mWriters;
			std::wstring_view mName;
		};
	}	
}

