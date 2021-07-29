#pragma once
#include "LogWriter.h"

namespace garam
{
	namespace logger
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
}