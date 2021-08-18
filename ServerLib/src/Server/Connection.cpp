#include "./Server/Connection.h"
#include "./Network/Socket.h"
#include "./Common/NetPacket.h"
#include "./Server/NetServer.h"
#include "Components/NetworkComponent.h"

namespace garam
{
	namespace net
	{		
		Connection::Connection(int id, NetworkComponent* network)
			: mNetworkComponent(network)
			, mSocket(nullptr)
			, mRecvBuffer(3000)
			, mSendBuffer(3000)			
			, mID(id)
			, mIsSending(false)
			, mInfo(this)
		{					
		}

		Connection::~Connection()
		{
		}

		void Connection::OnReceive(int recvBytes)
		{
			mRecvBuffer.MoveRear(recvBytes);
			
			/*
			 * �����ۿ��� ���ݱ��� ���� �����Ͱ� ������
			 * ���⼭ while���� ���鼭 ���� �̾Ƽ� ��Ŷ���� ������ ��
			 */
			while (mRecvBuffer.GetUseSize() > 0)
			{
				PacketHeader header;				
				NetPacket* packet = NetPacket::Alloc();
				packet->SetType(NetPacket::ePacketType::Receive);
				
				/*
				 * �켱 header�� ���� �� �ִ��� üũ
				 * header���� payload�� ����� �������
				 */
				if (!mRecvBuffer.Peek((char*)&header,
					sizeof(PacketHeader)))
				{
					NetPacket::Free(packet);
					break;
				}

				/*
				 * header + payload, �� ������ ��Ŷ�� ���� �� �ִ��� üũ
				 */
				if (!mRecvBuffer.Peek(packet->GetBuffer(),
					sizeof(PacketHeader) + header.length))
				{
					NetPacket::Free(packet);
					break;
				}

				/*
				 * �ϳ��� ��Ŷ�� ���� �� ������ �� ���̸�ŭ front �̵�
				 */
				mRecvBuffer.MoveFront(sizeof(PacketHeader) + header.length);
				packet->mSize = header.length;
								
				mNetworkComponent->OnPacketReceive(this, packet);
			}

			PostReceive();
		}

		void Connection::OnSent()
		{						
			int count = (int)mSentPackets.size();
			
			for (int i = 0; i < count; i++)
			{
				NetPacket* packet = mSentPackets.front();
				mSentPackets.pop();				
				
				NetPacket::Free(packet);
				mNetworkComponent->IncreaseSendTps();
			}
			
			InterlockedExchange(&mIsSending, false);

			if (mSendBuffer.GetUseSize() > 0)
			{				
				if (InterlockedExchange(&mIsSending, true))
				{
					return;
				}

				DataBuffer buf;
				PushPacketToDataBuffer(&buf);

				mSocket->AsyncSend(buf);
			}
		}

		void Connection::OnClose()
		{			
			mNetworkComponent->OnClose(this);
		}

		void Connection::PostReceive()
		{			
			mSocket->AsyncReceive(mRecvBuffer.GetBufferRear(), 
								  mRecvBuffer.GetDirectEnqueueSize());
		}

		void Connection::SendPacket(NetPacket* packet)
		{
			int length = packet->GetSize();
			packet->SetHeader(length);
			packet->AddRefCount();
			
			/*
			 * ����ȭ�� �ʿ�
			 * mSendBuffer�� ��Ƽ������� ������
			 */
			mSendBufferLock.lock();
			mSendBuffer.Enqueue((char*)&packet, sizeof(NetPacket*));
			mSendBufferLock.unlock();

			/*
			 * ��ȯ���� false�� ������ ���� ���� false�� ���¿��� true�� �ٲ� ��
			 * ���� �����ؾ� ��			 
			 */
			if (InterlockedExchange(&mIsSending, true))
			{
				return;
			}
			
			/*
			 * mSendBuffer�� �ִ°͵� �� Dequeue�ؼ� �����ϸ� �ȴ�.
			 */
			DataBuffer buf;
			PushPacketToDataBuffer(&buf);

			/*
			 * ���� ��Ŷ�� ���°Ŵϱ� �ٷ� return
			 */
			if (buf.GetCount() == 0)
			{
				InterlockedExchange(&mIsSending, false);
				return;
			}				
						
			mSocket->AsyncSend(buf);
		}

		void Connection::SetSocket(Socket* sock)
		{
			mSocket = sock;
			
			mSocket->RegisterReceiveCallback(std::bind(&Connection::OnReceive,
													   this,
													   std::placeholders::_1));

			mSocket->RegisterSendCallback(std::bind(&Connection::OnSent,
													this));

			mSocket->RegisterCloseCallback(std::bind(&Connection::OnClose,
													 this));
		}

		bool Connection::IsConnect()
		{
			return (mSocket != nullptr) && mSocket->IsConnect();
		}

		void Connection::Release()
		{			
			//mSocket->Close();
		}

		int Connection::GetID()
		{
			return mID;
		}

		Socket* Connection::GetSocket()
		{
			return mSocket;
		}

		std::wstring_view Connection::GetIPAddress()
		{
			return mSocket->GetIp();
		}

		ClientInfo* Connection::GetClientInfo()
		{
			return &mInfo;
		}

		void Connection::PushPacketToDataBuffer(DataBuffer* buffer)
		{									
			mSendBufferLock.lock();
			for (int i = 0; i < 100; i++)
			{				
				NetPacket* packet;
				if (!mSendBuffer.Dequeue((char*)&packet, sizeof(NetPacket*)))
				{															
					break;
				}
				
				mSentPackets.push(packet);

				buffer->Add(packet->GetBuffer(),
							packet->GetSize() + sizeof(PacketHeader));
			}				
			mSendBufferLock.unlock();
		}

		ClientInfo::ClientInfo(Connection* conn)
			: mConnection(conn)
		{
		}

		void ClientInfo::SendPacket(NetPacket* packet)
		{
			mConnection->SendPacket(packet);
		}

		int ClientInfo::GetID()
		{
			return mConnection->GetID();
		}

		bool ClientInfo::IsConnect()
		{			
			return mConnection->IsConnect();
		}

		std::wstring_view ClientInfo::GetClientIP()
		{
			return mConnection->GetIPAddress();
		}
	}
}