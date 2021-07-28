#pragma once
#include <string_view>
#include "LogWriter.h"

namespace logger
{
	class FileWriter : public Writer
	{
	public:
		FileWriter();
		~FileWriter();

		void Write(std::wstring_view loggerName, Log& log) override;

	private:		
	};
}