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
			GameServer(short port, int ccu);
			~GameServer();

		private:			
			void OnPacketReceive(Connection* conn, NetPacket* packet) override;
			void OnUpdate() override;

			std::mutex mPacketQueueLock;			
			std::queue<std::pair<Connection*, NetPacket*>> mPacketQueue;
			std::queue<std::pair<Connection*, NetPacket*>> mDispatchQueue;
		};
	}
}
