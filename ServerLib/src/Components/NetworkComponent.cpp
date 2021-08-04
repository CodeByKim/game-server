#include "./Components/NetworkComponent.h"
#include "Server/ConnectionManager.h"
#include "Common/NetPacket.h"
#include "Server/Connection.h"
#include "./Server/SocketAllocator.h"
#include "Server/NetServer.h"

namespace garam
{
	namespace net
	{
		NetworkComponent::NetworkComponent(NetServer* server, int ccu)
			: mServer(server)
		{
			mAcceptor.RegisterAcceptCallback(std::bind(&NetworkComponent::OnAccept,
														this,
														std::placeholders::_1));

			mAcceptor.SetAlloctor(new SocketAllocator());
			mConnectionManager = new ConnectionManager(this, ccu);
		}

		NetworkComponent::~NetworkComponent()
		{
		}

		void NetworkComponent::OnAccept(Socket* sock)
		{
			Connection* conn = mConnectionManager->Alloc();
			conn->SetSocket(sock);

			mServer->OnAccept(conn);
		}

		void NetworkComponent::OnPacketReceive(Connection* conn, NetPacket* packet)
		{
			mServer->OnPacketReceive(conn, packet);
		}

		void NetworkComponent::OnClose(Connection* conn)
		{
			mAcceptor.ReleaseSocket(conn->GetSocket());
			mConnectionManager->Free(conn);
			mServer->OnClose(conn);
		}

		void NetworkComponent::SendPacket(NetPacket* packet, ClientInfo* client)
		{
			//client->SendPacket(packet);			
		}

		void NetworkComponent::BroadCast(NetPacket* packet)
		{
			/*auto connections = mConnectionManager->GetConnections();
			for (int i = 0; i < connections.size(); i++)
			{
				connections[i]->SendPacket(packet);
			}*/
		}
	}
}
