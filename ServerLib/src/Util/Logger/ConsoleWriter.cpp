#include <iostream>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"
#include "./Util/Logger/ConsoleWriter.h"

namespace Log
{
	void ConsoleWriter::Write(InternalLogInfo* logInfo)
	{
		fwprintf(stdout,
				 L"[%s][%s]: %s\n",
				 logInfo->loggerName.data(),
				 LogLevelToString(logInfo->level).data(),
				 logInfo->stream->str().c_str());

		fflush(stdout);
	}
}