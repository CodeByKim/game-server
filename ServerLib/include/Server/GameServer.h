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
			void OnAccept(Socket* sock) override;
			void OnPacketReceive(Connection* conn, NetPacket* packet) override;
			void OnUpdate() override;
			void OnClose(Connection* conn) override;

			std::mutex mPacketQueueLock;			
			std::queue<std::pair<Connection*, NetPacket*>> mPacketQueue;
			std::queue<std::pair<Connection*, NetPacket*>> mDispatchQueue;
		};
	}
}
