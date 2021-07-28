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
		 * Logger �̸� �˾Ƴ���
		 * Log ���� �˾Ƴ���
		 * ��� �ð� �˾Ƴ���
		 * �ѹ濡 ���
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