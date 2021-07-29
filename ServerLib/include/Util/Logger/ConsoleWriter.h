#pragma once
#include "LogWriter.h"

namespace garam
{
	namespace logger
	{
		class ConsoleWriter : public Writer
		{
		public:

		protected:
			void Write(InternalLogInfo* logInfo) override;

		private:

		};
	}	
}