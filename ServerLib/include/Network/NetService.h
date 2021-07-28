#pragma once
#include "../Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		class Socket;		

		class NetService
		{
		public:
			NetService();
			~NetService();

			void							AssociateCompletePort(SOCKET handle, Socket* sock);			

		private:
			void							WorkerThread();

			HANDLE							mIocp;
			std::vector<std::thread>		mWorkerThreads;
			int								mWorkerThreadCount;				
		};
	}
}