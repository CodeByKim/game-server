#pragma once
#include "../Common/CommonLib.h"
#include "../Common/NetPacket.h"
#include "./NetServer.h"

namespace garam
{
	namespace net
	{
		class GameLogic;

		class GameServer : public NetServer
		{
		public:			
			GameServer(short port, int ccu, IMessageHandler* handler, GameLogic* logic);
			~GameServer();
			
		private:	
			void OnAccept(Connection* conn) override;
			void OnPacketReceive(Connection* conn, NetPacket* packet) override;
			void OnUpdate() override;
			void OnClose(Connection* conn) override;			

			std::mutex mPacketQueueLock;			
			std::queue<std::pair<Connection*, NetPacket*>> mPacketQueue;
			std::queue<std::pair<Connection*, NetPacket*>> mDispatchQueue;
			GameLogic* mLogic;
		};
	}
}
