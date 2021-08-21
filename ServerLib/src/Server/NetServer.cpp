#include "./Server/NetServer.h"
#include "./Network/Acceptor.h"
#include "./Network/Socket.h"
#include "./Server/Connection.h"
#include "./Common/NetPacket.h"
#include "./Server/IMessageHandler.h"
#include "./Components/NetworkComponent.h"

namespace garam
{
	namespace net
	{					
		NetServer::NetServer(short port, int ccu)
			: mMessageHandler(nullptr)			
			, mNetworkComponent(nullptr)
		{			
			InitLogger();
			time::Time::Initialize();

			mNetworkComponent = new NetworkComponent(this, ccu);			
		}

		NetServer::~NetServer()
		{
		}

		void NetServer::Run()
		{
			while (true)
			{				
				time::Time::Update();

				mNetworkComponent->OnUpdate(time::Time::GetDeltaTime());
				OnUpdate();
				mMessageHandler->OnUpdate(time::Time::GetDeltaTime());

				Sleep(1);
			}
		}

		void NetServer::RegisterMessageHandler(IMessageHandler* handler)
		{
			mMessageHandler = handler;
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

		void NetServer::OnAccept(Connection* conn)
		{			
			mMessageHandler->OnClientJoin(conn->GetClientInfo());
			conn->PostReceive();
		}

		void NetServer::OnUpdate()
		{			
		}

		void NetServer::OnPacketReceive(Connection* conn, NetPacket* packet)
		{					
			mMessageHandler->OnPacketReceive(conn->GetClientInfo(), packet);
			NetPacket::Free(packet);
		}

		void NetServer::OnClose(Connection* conn)
		{			
			mMessageHandler->OnClientLeave(conn->GetClientInfo());			
		}
	}
}