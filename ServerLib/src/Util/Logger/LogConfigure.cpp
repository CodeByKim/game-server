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
		 * �ߺ����� writer �߰� ���ϵ��� ����
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
