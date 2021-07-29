#pragma once
#include <sstream>

namespace Log
{
	enum class eLogLevel
	{
		Info,
		Error,
	};

	class Logger;
	enum class eLogLevel;

	std::wstring_view LogLevelToString(eLogLevel level);

	class Log
	{	
	public:		
		template <typename T>
		Log& operator<< (T value)
		{
			*mStream << value;
			return (*this);
		}

		Log(Logger& logger, eLogLevel level);
		~Log();

		eLogLevel mLevel;
		std::wostringstream* mStream;

	private:
		Logger& mLogger;		
	};

//end of namespace
}