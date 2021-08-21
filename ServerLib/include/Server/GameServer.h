#pragma once
#include "../Common/CommonLib.h"
#include "../Common/NetPacket.h"
#include "./NetServer.h"

class World;

namespace garam
{
	namespace net
	{
		class GameServer : public NetServer
		{
		public:			
			GameServer(short port, int ccu);
			~GameServer();

			static void RegisterGameWorld(World* world);

		private:	
			void OnAccept(Connection* conn) override;
			void OnPacketReceive(Connection* conn, NetPacket* packet) override;
			void OnUpdate() override;
			void OnClose(Connection* conn) override;
			void HandlePacket();

			std::mutex mPacketQueueLock;			
			std::queue<std::pair<Connection*, NetPacket*>> mPacketQueue;
			std::queue<std::pair<Connection*, NetPacket*>> mDispatchQueue;
			static World* mGameWorld;
		};
	}
}
