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
			void IncreaseSendTps();
			void IncreaseRecvTps();
			void OnUpdate(float deltaTime);

		private:
			NetServer* mServer;
			Acceptor mAcceptor;
			ConnectionManager* mConnectionManager;
			float mTpsDelay;
			ULONGLONG mSendTps;
			ULONGLONG mRecvTps;
		};
	}
}