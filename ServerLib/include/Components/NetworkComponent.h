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
			
			void SendPacket(NetPacket* packet, ClientInfo* client);
			void BroadCast(NetPacket* packet);
			
			void OnAccept(Socket* sock);
			void OnPacketReceive(Connection* conn, NetPacket* packet);
			void OnClose(Connection* conn);

		private:			
			NetServer* mServer;
			ConnectionManager* mConnectionManager;
			Acceptor mAcceptor;			
		};
	}
}


