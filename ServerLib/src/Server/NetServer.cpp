#include "./Server/NetServer.h"
#include "./Network/Acceptor.h"
#include "./Network/Socket.h"
#include "./Server/Connection.h"
#include "./Common/NetPacket.h"
#include "./Server/SocketAllocator.h"
#include "./Server/IMessageHandler.h"
#include "./Util/Time/Time.h"

namespace garam
{
	namespace net
	{				
		NetServer::NetServer(short port, int ccu)
			: mMessageHandler(nullptr)
			, mConnectionManager(this, ccu)
		{			
			InitLogger();
			time::Time::Initialize();

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
				time::Time::Update();

				OnUpdate();
				mMessageHandler->OnUpdate(time::Time::GetDeltaTime());

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

		void NetServer::OnUpdate()
		{
			
		}

		void NetServer::OnClose(Connection* conn)
		{
			mAcceptor.ReleaseSocket(conn->GetSocket());
			mMessageHandler->OnClientLeave(conn->GetClientInfo());
			mConnectionManager.Free(conn);			
		}

		void NetServer::OnPacketReceive(Connection* conn, NetPacket* packet)
		{					
			mMessageHandler->OnPacketReceive(conn->GetClientInfo(), packet);
			NetPacket::Free(packet);
		}

		void NetServer::InitLogger()
		{
			logger::Configure networkLogConfig;
			networkLogConfig.SetLoggerName(L"Network");
			networkLogConfig.SetWriter(logger::eLogWriter::Console);			

			logger::Configure serverLogConfig;
			serverLogConfig.SetLoggerName(L"Server");
			serverLogConfig.SetWriter(logger::eLogWriter::Console);			

			logger::Manager::Create(&networkLogConfig);
			logger::Manager::Create(&serverLogConfig);
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