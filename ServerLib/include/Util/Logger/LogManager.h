#pragma once
#include <vector>
#include <unordered_map>

#include "LogConfigure.h"

namespace Log
{
	#define LOG_INFO(LOGGER) Log::Manager::Get(LOGGER)->Info()
	#define LOG_ERROR(LOGGER) Log::Manager::Get(LOGGER)->Error()

	class Log;
	class Logger;
	class Writer;

	class Manager
	{
	public:
		static void Create(Configure* config);		
		static Logger* Get(std::wstring_view name);
		static void Release();

	private:				
		static std::unordered_map<std::wstring_view, Logger*> mLoggers;
		static std::unordered_map<eLogWriter, Writer*> mWriters;
	};
}
