#pragma once
#include "LogWriter.h"

namespace Log
{
	class ConsoleWriter : public Writer
	{
	public:
				
	protected:
		void Write(InternalLogInfo* logInfo) override;

	private:

	};
}