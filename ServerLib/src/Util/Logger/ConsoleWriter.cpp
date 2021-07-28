#include <iostream>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/ConsoleWriter.h"

namespace logger
{
	void ConsoleWriter::Write(std::wstring_view loggerName, Log& log)
	{
		/*
		 * Logger �̸� �˾Ƴ���
		 * Log ���� �˾Ƴ���
		 * ��� �ð� �˾Ƴ���
		 * �ѹ濡 ���
		 */
		
		fwprintf(stdout, 
				 L"[%s][%s]: %s\n", 
				 loggerName.data(), 
				 LogLevelToString(log.mLevel).data(),
				 log.mStream->str().c_str());

		fflush(stdout);
	}
}