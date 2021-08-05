#pragma once
#include "Common/CommonLib.h"
#include "Network/Acceptor.h"

namespace garam
{
	namespace net
	{
		class ConnectionManager;
		class NetPacket;
		class ClientInfo;
		class NetServer;
		class Connection;

		class NetworkComponent
		{
		public:
			NetworkComponent(NetServer* server, int ccu);
			~NetworkComponent();
			
			void OnAccept(Socket* sock);
			void OnPacketReceive(Connection* conn, NetPacket* packet);
			void OnClose(Connection* conn);
			
			static void BroadCast(NetPacket* packet);
			static void BroadCast(NetPacket* packet, ClientInfo* exceptClient);

		private:			
			NetServer* mServer;			
			Acceptor mAcceptor;			
			static ConnectionManager* mConnectionManager;
		};
	}
}