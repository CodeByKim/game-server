#include <Windows.h>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/OutputDebugWriter.h"

namespace logger
{
	void OutputDebugWriter::Write(std::wstring_view loggerName, Log& log)
	{
		/*
		 * Logger 이름 알아내고
		 * Log 레벨 알아내고
		 * 출력 시간 알아내서
		 * 한방에 출력
		 */

		WCHAR str[128];
		wsprintf(str, 
				 L"[%s][%s]: %s\n", 
				 loggerName.data(), 
				 LogLevelToString(log.mLevel).data(), 
				 log.mStream->str().c_str());

		OutputDebugString(str);
	}
}