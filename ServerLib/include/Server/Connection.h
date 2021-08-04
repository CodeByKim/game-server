#pragma once
#include "../Common/CommonLib.h"
#include "../Common/RingBuffer.h"

namespace garam
{
	namespace net
	{
		class Socket;
		class NetPacket;
		class NetServer;		
		class NetworkComponent;
		class DataBuffer;						
		class Connection;

		class ClientInfo
		{
		public:
			ClientInfo(Connection* conn);
			
			//TODO : 나중엔 제거대상, NetworkComponent로 이동 예정
			void SendPacket(NetPacket* packet);
			int GetID();
			bool IsConnect();
			std::wstring_view GetClientIP();

		private:
			Connection* mConnection;
		};

		class Connection
		{
		public:			
			Connection(int id, NetworkComponent* network);
			~Connection();

			void						OnReceive(int recvBytes);
			void						OnSent();
			void						OnClose();
			void						PostReceive();
			void						SendPacket(NetPacket* packet);
			void						SetSocket(Socket* sock);
			bool						IsConnect();
			void						Release();
			int							GetID();
			Socket*						GetSocket();
			std::wstring_view			GetIPAddress();
			ClientInfo*					GetClientInfo();

		private:			
			void						PushPacketToDataBuffer(DataBuffer* buffer);

			Socket*						mSocket;			
			//NetServer*					mServer;
			NetworkComponent*			mNetworkComponent;
			RingBuffer					mRecvBuffer;
			RingBuffer					mSendBuffer;
			std::queue<NetPacket*>		mSentPackets;
			std::mutex					mSendBufferLock;	
			ULONGLONG					mIsSending;
			int							mID;

			ClientInfo					mInfo;
		};
	}
}