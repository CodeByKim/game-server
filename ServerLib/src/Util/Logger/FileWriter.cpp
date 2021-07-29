#include <fstream>

#include "./Util/Logger/Log.h"
#include "./Util/Logger/Logger.h"
#include "./Util/Logger/FileWriter.h"

namespace garam
{
	namespace logger
	{
		FileWriter::FileWriter()
		{

		}

		FileWriter::~FileWriter()
		{

		}

		void FileWriter::Write(InternalLogInfo* logInfo)
		{
			FILE* file;
			fopen_s(&file, "log.txt", "at");

			if (file == nullptr)
				return;

			fwprintf(file,
				L"[%s][%s]: %s\n",
				logInfo->loggerName.data(),
				LogLevelToString(logInfo->level).data(),
				logInfo->stream->str().c_str());

			fflush(file);
			fclose(file);
		}
	}	
}