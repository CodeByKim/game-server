#include "./Server/Connection.h"
#include "./Network/Socket.h"
#include "./Common/NetPacket.h"
#include "./Server/NetServer.h"

namespace garam
{
	namespace net
	{		
		Connection::Connection(int id, NetServer* server)
			: mServer(server)
			, mSocket(nullptr)
			, mRecvBuffer(1500)
			, mSendBuffer(1500)
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
			 * 링버퍼에는 지금까지 받은 데이터가 차있음
			 * 여기서 while문을 돌면서 전부 뽑아서 패킷으로 만들어야 함
			 */
			while (mRecvBuffer.GetUseSize() > 0)
			{
				PacketHeader header;				
				NetPacket* packet = NetPacket::Alloc();

				/*
				 * 우선 header를 읽을 수 있는지 체크
				 * header에는 payload의 사이즈가 들어있음
				 */
				if (!mRecvBuffer.Peek((char*)&header,
					sizeof(PacketHeader)))
				{
					NetPacket::Free(packet);
					break;
				}

				/*
				 * header + payload, 즉 온전한 패킷을 읽을 수 있는지 체크
				 */
				if (!mRecvBuffer.Peek(packet->GetBuffer(),
					sizeof(PacketHeader) + header.length))
				{
					NetPacket::Free(packet);
					break;
				}

				/*
				 * 하나의 패킷을 읽을 수 있으면 그 길이만큼 front 이동
				 */
				mRecvBuffer.MoveFront(sizeof(PacketHeader) + header.length);
				packet->mSize = header.length;
														
				mServer->OnPacketReceive(this, packet);				
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
			mServer->OnClose(this);
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

			/*
			 * 동기화가 필요
			 * mSendBuffer가 멀티스레드로 동작함
			 */
			mSendBufferLock.lock();

			packet->AddRefCount();
			mSendBuffer.Enqueue((char*)&packet, sizeof(NetPacket*));

			mSendBufferLock.unlock();

			/*
			 * 반환값이 false가 나오면 내가 직접 false인 상태에서 true로 바꾼 것
			 * 실제 전송해야 함			 
			 */
			if (InterlockedExchange(&mIsSending, true))
			{
				return;
			}
			
			/*
			 * mSendBuffer에 있는것들 다 Dequeue해서 전송하면 된다.
			 */
			DataBuffer buf;
			PushPacketToDataBuffer(&buf);

			/*
			 * 보낼 패킷이 없는거니까 바로 return
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
			return mSocket->IsConnect();
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

		std::string_view Connection::GetIPAddress()
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

		std::string_view ClientInfo::GetClientIP()
		{
			return mConnection->GetIPAddress();
		}
	}
}