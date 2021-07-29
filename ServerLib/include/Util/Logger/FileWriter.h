#pragma once
#include "LogWriter.h"

namespace Log
{
	class FileWriter : public Writer
	{
	public:
		FileWriter();
		~FileWriter();

	protected:		
		void Write(InternalLogInfo* logInfo) override;

	private:		
	};
}