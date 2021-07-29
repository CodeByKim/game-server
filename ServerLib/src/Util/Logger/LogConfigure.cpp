#include "./Util/Logger/LogConfigure.h"

namespace Log
{
	void Configure::SetLoggerName(std::wstring_view loggerName)
	{
		mLoggerName = loggerName;
	}

	void Configure::SetWriter(eLogWriter writer)
	{
		/*
		 * 중복으로 writer 추가 못하도록 방지
		 */
		for (auto iter = mWriters.begin();
			iter != mWriters.end();
			++iter)
		{
			if (*iter == writer)
				return;
		}

		mWriters.push_back(writer);
	}
}
