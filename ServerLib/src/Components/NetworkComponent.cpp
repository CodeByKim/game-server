#include "./Components/NetworkComponent.h"

namespace garam
{
	namespace net
	{
		void NetworkComponent::AddDependency(ConnectionManager* manager)
		{
			mConnectionManager = manager;
		}

		void NetworkComponent::SendPacket(NetPacket* packet, ClientInfo* client)
		{
			client->SendPacket(packet);
		}

		void NetworkComponent::BroadCast(NetPacket* packet)
		{
			auto connections = mConnectionManager->GetConnections();
			for (int i = 0; i < connections.size(); i++)
			{
				connections[i]->SendPacket(packet);
			}
		}
	}
}
