#pragma once
#include <string_view>
#include "Log.h"
#include "LogConfigure.h"

namespace logger
{
	class Wrtier;

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

