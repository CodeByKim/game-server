#include "./Server/ConnectionManager.h"
#include "./Server/Connection.h"

namespace garam
{
	namespace net
	{
		ConnectionManager::ConnectionManager(NetServer* server, int ccu)
			: mMaxCCU(ccu)
			, mServer(server)
			, mCCU(0)
		{
			for (int i = 0; i < mMaxCCU; i++)
			{
				mConnections.push_back(new Connection(i, mServer));
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
			mCCU -= 1;

			mEmptyConnectionIndex.push(index);
		}

		int ConnectionManager::GetCCU()
		{
			return mCCU;
		}

		/*
		Connection& ConnectionManager::FindConnectionById(int id)
		{
			for (int i = 0; i < mConnectionCount; i++)
			{
				if (mConnections[i].mID == id)
				{
					return mConnections[i];
				}
			}
		}
		*/
	}		
}