#pragma once
#include "LogWriter.h"

namespace logger
{
	class ConsoleWriter : public Writer
	{
	public:
		void Write(std::wstring_view loggerName, Log& log) override;

	private:

	};
}