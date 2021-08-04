#pragma once
#include "Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		class ConnectionManager;
		class NetPacket;
		class ClientInfo;

		class NetworkComponent
		{
		public:
			NetworkComponent();
			~NetworkComponent();

			void AddDependency(ConnectionManager* manager);
			static void SendPacket(NetPacket* packet, ClientInfo* client);
			static void BroadCast(NetPacket* packet);

		private:
			static ConnectionManager* mConnectionManager;
		};
	}
}


