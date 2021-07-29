#pragma once
#include "LogWriter.h"

namespace Log
{
	class OutputDebugWriter : public Writer
	{
	public:		

	protected:		
		void Write(InternalLogInfo* logInfo) override;

	private:

	};
}