#pragma once
#include "Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		class NetworkComponent
		{
		public:
			NetworkComponent();
			void AddDependency(ConnectionManager* manager);
			static void SendPacket(NetPacket* packet, ClientInfo* client);
			static void BroadCast(NetPacket* packet);

		private:
			static ConnectionManager* mConnectionManager;
		};
	}
}


