#include "./Server/GameServer.h"
#include "./Server/IMessageHandler.h"
#include "./Server/Connection.h"

namespace garam
{
	namespace net
	{
		GameServer::GameServer(short port, int ccu)
			: NetServer(port, ccu)
		{
		}

		GameServer::~GameServer()
		{
		}

		void GameServer::OnPacketReceive(Connection* conn, NetPacket* packet)
		{			
			std::scoped_lock<std::mutex> lock(mPacketQueueLock);
			
			mPacketQueue.push(std::make_pair(conn, packet));
		}

		void GameServer::OnUpdate()
		{
			mPacketQueueLock.lock();
			std::swap(mPacketQueue, mDispatchQueue);
			mPacketQueueLock.unlock();
					
			for (int i = 0; i < mDispatchQueue.size(); i++)
			{
				auto package = mDispatchQueue.front();
				mDispatchQueue.pop();

				mMessageHandler->OnPacketReceive(package.first->GetClientInfo(), package.second);
				NetPacket::Free(package.second);
			}			
		}
	}
}