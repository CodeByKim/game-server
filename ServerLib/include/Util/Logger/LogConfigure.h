#pragma once
#include <string_view>
#include <vector>
#include "LogWriter.h"

namespace Log
{
	class Writer;	

	class Configure
	{
	public:
		friend class Logger;
		friend class Manager;

		void SetLoggerName(std::wstring_view loggerName);
		void SetWriter(eLogWriter writer);

	private:
		std::wstring_view mLoggerName;
		std::vector<eLogWriter> mWriters;
	};
}
