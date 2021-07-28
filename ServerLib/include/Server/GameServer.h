#pragma once
#include "../Common/CommonLib.h"
#include "../Common/NetPacket.h"
#include "./NetServer.h"

namespace garam
{
	namespace net
	{
		class GameServer : public NetServer
		{
		public:			
			~GameServer();

			static GameServer* Create(short port, int ccu);

		private:
			GameServer(short port, int ccu);

			void OnPacketReceive(Connection* conn, NetPacket* packet) override;
			void OnUpdate() override;

			std::mutex mPacketQueueLock;			
			std::queue<std::pair<Connection*, NetPacket*>> mPacketQueue;
			std::queue<std::pair<Connection*, NetPacket*>> mDispatchQueue;
		};
	}
}
