#include <fstream>
#include "./Util/Logger/Log.h"
#include "./Util/Logger/FileWriter.h"

namespace logger
{
	FileWriter::FileWriter()
	{
		
	}

	FileWriter::~FileWriter()
	{

	}

	void FileWriter::Write(std::wstring_view loggerName, Log& log)
	{		
		/*
		 * Logger 이름 알아내고
		 * Log 레벨 알아내고
		 * 출력 시간 알아내서
		 * 한방에 출력
		 */

		FILE* file;
		fopen_s(&file, "log.txt", "at");

		if (file == nullptr)
			return;

		fwprintf(file,
			L"[%s][%s]: %s\n",
			loggerName.data(),
			LogLevelToString(log.mLevel).data(),
			log.mStream->str().c_str());

		fflush(file);
		fclose(file);
	}
}