#pragma once
#include "../Common/CommonLib.h"
#include "../Common/MemoryPool.h"
#include "../Common/IAllacator.h"

namespace garam
{
	namespace net
	{
		class Connection;		
		class NetServer;

		class ConnectionManager : public IAllocator<Connection*>
		{
		public:
			ConnectionManager(NetServer* server, int ccu);
			~ConnectionManager();

			Connection* Alloc();
			void Free(Connection* conn);
			int GetCCU();

		private:						
			std::vector<Connection*> mConnections;			
			NetServer* mServer;
			int mMaxCCU;
			int mCCU;
			std::stack<int> mEmptyConnectionIndex;
			std::mutex mEmptyConnectionIndexLock;
		};
	}	
}