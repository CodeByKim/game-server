#pragma once
#include "../Common/CommonLib.h"
#include "../Common/MemoryPool.h"
#include "../Common/IAllacator.h"

namespace garam
{
	namespace net
	{
		class Connection;		
		class ClientInfo;
		class NetworkComponent;

		class ConnectionManager : public memory::IAllocator<Connection*>
		{
		public:
			ConnectionManager(NetworkComponent* network, int ccu);
			~ConnectionManager();

			Connection* Alloc();
			void Free(Connection* conn);
			std::vector<Connection*>& GetConnections();			
			int GetCCU();

		private:						
			std::vector<Connection*> mConnections;
			std::stack<int> mEmptyConnectionIndex;
			std::mutex mEmptyConnectionIndexLock;
			NetworkComponent* mNetworkComponent;
			int mMaxCCU;
			int mCCU;			
		};
	}	
}