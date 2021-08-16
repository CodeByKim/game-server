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
			, mTpsDelay(0)
			, mSendTps(0)
			, mRecvTps(0)
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
			IncreaseRecvTps();

			mServer->OnPacketReceive(conn, packet);
		}

		void NetworkComponent::OnClose(Connection* conn)
		{
			mAcceptor.ReleaseSocket(conn->GetSocket());			
			mConnectionManager->Free(conn);
			mServer->OnClose(conn);			
		}
		
		void NetworkComponent::IncreaseSendTps()
		{
			InterlockedIncrement(&mSendTps);
		}

		void NetworkComponent::IncreaseRecvTps()
		{
			InterlockedIncrement(&mRecvTps);
		}

		void NetworkComponent::OnUpdate(float deltaTime)
		{
			if (mTpsDelay <= 0)
			{
				/*printf("send tps : %d\n", (int)mSendTps);
				printf("recv tps : %d\n", (int)mRecvTps);*/

				InterlockedExchange(&mSendTps, 0);
				InterlockedExchange(&mRecvTps, 0);

				mTpsDelay = 1;
			}
			else
			{
				mTpsDelay -= deltaTime;
			}
		}
	}
}
