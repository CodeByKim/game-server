#pragma once
#include "LogWriter.h"

namespace garam
{
	namespace logger
	{
		class OutputDebugWriter : public Writer
		{
		public:

		protected:
			void Write(InternalLogInfo* logInfo) override;

		private:

		};
	}	
}