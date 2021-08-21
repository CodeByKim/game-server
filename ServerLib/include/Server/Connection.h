#pragma once
#include "../Common/CommonLib.h"
#include "../Common/RingBuffer.h"

class World;

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
			friend class GameServer;

			ClientInfo(Connection* conn);
						
			void SendPacket(NetPacket* packet);
			int GetID();
			bool IsConnect();
			std::wstring_view GetClientIP();
			World* GetWorld();

		private:
			Connection* mConnection;
			World* mWorld;
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