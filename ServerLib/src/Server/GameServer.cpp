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

		GameServer* GameServer::Create(short port, int ccu)
		{
			static GameServer server(port, ccu);
			return &server;
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
				PacketAllocator::GetInstance().Free(package.second);
			}
		}
	}
}