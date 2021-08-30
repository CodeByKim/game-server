#include "./Server/ConnectionManager.h"
#include "./Server/Connection.h"

namespace garam
{
	namespace net
	{
		ConnectionManager::ConnectionManager(NetworkSensor* network, int ccu)
			: mMaxCCU(ccu)
			, mNetworkSensor(network)
			, mCCU(0)
		{
			for (int i = 0; i < mMaxCCU; i++)
			{
				mConnections.push_back(new Connection(i, mNetworkSensor));
				mEmptyConnectionIndex.push(i);
			}
		}

		ConnectionManager::~ConnectionManager()
		{
		}

		Connection* ConnectionManager::Alloc()
		{						
			std::scoped_lock<std::mutex> lock(mEmptyConnectionIndexLock);
			
			int index = mEmptyConnectionIndex.top();
			mEmptyConnectionIndex.pop();
			mCCU += 1;

			return mConnections[index];
		}

		void ConnectionManager::Free(Connection* conn)
		{			
			std::scoped_lock<std::mutex> lock(mEmptyConnectionIndexLock);
			
			conn->Release();
			int index = conn->GetID();
			
			mEmptyConnectionIndex.push(index);
			mCCU -= 1;
		}

		std::vector<Connection*>& ConnectionManager::GetConnections()
		{
			return mConnections;
		}

		/*std::vector<Connection*>& ConnectionManager::GetConnectedConnections()
		{
			return mConnectedConnections;
		}*/

		int ConnectionManager::GetCCU()
		{
			return mCCU;
		}
	}		
}