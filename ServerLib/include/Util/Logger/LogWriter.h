#pragma once

namespace logger
{
	enum class eLogWriter
	{
		Console,
		OutputDebug,
		File
	};

	class Log;

	class Writer
	{
	public:
		virtual void Write(std::wstring_view loggerName, Log& log) = 0;
	};

/*
 * end of namespace
 */
}