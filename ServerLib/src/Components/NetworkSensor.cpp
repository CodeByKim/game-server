#include "./Components/NetworkSensor.h"
#include "Server/ConnectionManager.h"
#include "Common/NetPacket.h"
#include "Server/Connection.h"
#include "./Server/SocketAllocator.h"
#include "Server/NetServer.h"

namespace garam
{
	namespace net
	{		
		NetworkSensor::NetworkSensor(NetServer* server, int ccu)
			: mServer(server)
			, mTpsDelay(0)
			, mSendTps(0)
			, mRecvTps(0)
		{
			mAcceptor.RegisterAcceptCallback(std::bind(&NetworkSensor::OnAccept,
														this,
														std::placeholders::_1));

			mAcceptor.SetAlloctor(new SocketAllocator());
			mConnectionManager = new ConnectionManager(this, ccu);
		}

		NetworkSensor::~NetworkSensor()
		{
		}

		void NetworkSensor::OnAccept(Socket* sock)
		{
			Connection* conn = mConnectionManager->Alloc();
			conn->SetSocket(sock);

			mServer->OnAccept(conn);
		}

		void NetworkSensor::OnPacketReceive(Connection* conn, NetPacket* packet)
		{
			IncreaseRecvTps();

			mServer->OnPacketReceive(conn, packet);
		}

		void NetworkSensor::OnClose(Connection* conn)
		{
			mAcceptor.ReleaseSocket(conn->GetSocket());			
			mConnectionManager->Free(conn);
			mServer->OnClose(conn);			
		}
		
		void NetworkSensor::IncreaseSendTps()
		{
			InterlockedIncrement(&mSendTps);
		}

		void NetworkSensor::IncreaseRecvTps()
		{
			InterlockedIncrement(&mRecvTps);
		}

		void NetworkSensor::OnUpdate(float deltaTime)
		{
			if (mTpsDelay <= 0)
			{
				printf("send tps : %d\n", (int)mSendTps);
				printf("recv tps : %d\n", (int)mRecvTps);

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
