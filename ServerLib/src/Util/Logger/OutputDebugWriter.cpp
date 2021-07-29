#include <Windows.h>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"
#include "./Util/Logger/OutputDebugWriter.h"
#include <iostream>

namespace garam
{
	namespace logger
	{
		void OutputDebugWriter::Write(InternalLogInfo* logInfo)
		{
			WCHAR str[128];
			wsprintf(str,
				L"[%s][%s]: %s\n",
				logInfo->loggerName.data(),
				LogLevelToString(logInfo->level).data(),
				logInfo->stream->str().c_str());

			OutputDebugString(str);
		}
	}	
}