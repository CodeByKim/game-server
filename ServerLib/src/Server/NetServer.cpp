#include "./Server/NetServer.h"
#include "./Network/Acceptor.h"
#include "./Network/Socket.h"
#include "./Server/Connection.h"
#include "./Common/NetPacket.h"
#include "./Server/SocketAllocator.h"
#include "./Server/IMessageHandler.h"

namespace garam
{
	namespace net
	{				
		NetServer::NetServer(short port, int ccu)
			: mMessageHandler(nullptr)
			, mConnectionManager(this, ccu)
		{			
			mAcceptor.RegisterAcceptCallback(std::bind(&NetServer::OnAccept,
													   this,
													   std::placeholders::_1));	

			mAcceptor.SetAlloctor(new SocketAllocator());			
		}

		NetServer::~NetServer()
		{
		}

		void NetServer::Run()
		{
			while (true)
			{				
				OnUpdate();
				mMessageHandler->OnUpdate();
				Sleep(1);
			}
		}

		//void NetServer::SendPacket(int id, NetPacket* packet)
		//{			
		//	int length = packet->GetSize();
		//	packet->SetHeader(length);
		
		//	gConnectionsLock.lock();

		//	for (int i = 0; i < gConnections.size(); i++)
		//	{
		//		if (gConnections[i]->GetID() == id)
		//		{
		//			gConnections[i]->SendPacket(packet);
		//			gConnectionsLock.unlock();
		//			return;
		//		}
		//	}

		//	gConnectionsLock.unlock();
		//}

		void NetServer::RegisterMessageHandler(IMessageHandler* handler)
		{
			mMessageHandler = handler;
		}

		NetServer* NetServer::Create(short port, int ccu)
		{
			static NetServer server(port, ccu);
			return &server;
		}

		void NetServer::OnUpdate()
		{
			
		}

		void NetServer::OnClose(Connection* conn)
		{
			mAcceptor.ReleaseSocket(conn->GetSocket());
			mConnectionManager.Free(conn);
		}

		void NetServer::OnPacketReceive(Connection* conn, NetPacket* packet)
		{					
			mMessageHandler->OnPacketReceive(conn->GetClientInfo(), packet);
			PacketAllocator::GetInstance().Free(packet);
		}

		void NetServer::OnAccept(Socket* sock)
		{			
			Connection* conn = mConnectionManager.Alloc();
			conn->SetSocket(sock);
			
			mMessageHandler->OnClientJoin(conn->GetClientInfo());			
			conn->PostReceive();
		}
	}
}