#include <Windows.h>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/OutputDebugWriter.h"

namespace logger
{
	void OutputDebugWriter::Write(std::wstring_view loggerName, Log& log)
	{
		/*
		 * Logger �̸� �˾Ƴ���
		 * Log ���� �˾Ƴ���
		 * ��� �ð� �˾Ƴ���
		 * �ѹ濡 ���
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