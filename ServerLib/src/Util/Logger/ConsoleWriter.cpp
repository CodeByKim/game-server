#include <iostream>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/ConsoleWriter.h"

namespace logger
{
	void ConsoleWriter::Write(std::wstring_view loggerName, Log& log)
	{
		/*
		 * Logger 이름 알아내고
		 * Log 레벨 알아내고
		 * 출력 시간 알아내서
		 * 한방에 출력
		 */
		
		fwprintf(stdout, 
				 L"[%s][%s]: %s\n", 
				 loggerName.data(), 
				 LogLevelToString(log.mLevel).data(),
				 log.mStream->str().c_str());

		fflush(stdout);
	}
}