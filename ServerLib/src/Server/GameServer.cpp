#include "./Server/GameServer.h"
#include "./Server/IMessageHandler.h"
#include "./Server/Connection.h"
#include "Components/NetworkComponent.h"
#include "./Contents/World.h"

namespace garam
{
	namespace net
	{
		World* GameServer::mGameWorld = nullptr;

		GameServer::GameServer(short port, int ccu)
			: NetServer(port, ccu)			
		{
		}

		GameServer::~GameServer()
		{
		}

		void GameServer::RegisterGameWorld(World* world)
		{
			mGameWorld = world;
		}

		void GameServer::OnAccept(Connection* conn)
		{						
			NetPacket* packet = NetPacket::Alloc();
			packet->SetType(NetPacket::ePacketType::Accept);

			mPacketQueueLock.lock();
			mPacketQueue.push(std::make_pair(conn, packet));
			mPacketQueueLock.unlock();

			conn->PostReceive();
		}

		void GameServer::OnPacketReceive(Connection* conn, NetPacket* packet)
		{			
			std::scoped_lock<std::mutex> lock(mPacketQueueLock);
			
			mPacketQueue.push(std::make_pair(conn, packet));
		}

		void GameServer::OnUpdate()
		{
			HandlePacket();

			if (mGameWorld != nullptr)
			{
				mGameWorld->OnUpdate(time::Time::GetDeltaTime());
			}				
		}

		void GameServer::OnClose(Connection* conn)
		{			
			NetPacket* packet = NetPacket::Alloc();
			packet->SetType(NetPacket::ePacketType::Disconnect);

			mPacketQueueLock.lock();
			mPacketQueue.push(std::make_pair(conn, packet));
			mPacketQueueLock.unlock();			
		}

		void GameServer::HandlePacket()
		{
			mPacketQueueLock.lock();
			std::swap(mPacketQueue, mDispatchQueue);
			mPacketQueueLock.unlock();

			for (int i = 0; i < mDispatchQueue.size(); i++)
			{
				auto package = mDispatchQueue.front();
				mDispatchQueue.pop();

				ClientInfo* client = package.first->GetClientInfo();
				NetPacket* packet = package.second;

				switch (packet->GetType())
				{
				case NetPacket::ePacketType::Accept:
					client->mWorld = mGameWorld;
					mMessageHandler->OnClientJoin(client);
					break;

				case NetPacket::ePacketType::Receive:
					mMessageHandler->OnPacketReceive(client, packet);
					break;

				case NetPacket::ePacketType::Disconnect:
					mMessageHandler->OnClientLeave(client);
					break;
				}

				NetPacket::Free(package.second);
			}
		}
	}
}