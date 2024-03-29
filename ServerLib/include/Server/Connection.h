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
		class NetworkSensor;
		class DataBuffer;						
		class Connection;
		//class World;

		class ClientInfo
		{
		public:
			friend class GameServer;

			ClientInfo(Connection* conn);
						
			void SendPacket(NetPacket* packet);
			int GetID();
			bool IsConnect();
			std::wstring_view GetClientIP();
			//World* GetWorld();

		private:
			Connection* mConnection;
			//World* mWorld;
		};

		class Connection
		{
		public:			
			Connection(int id, NetworkSensor* network);
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
			NetworkSensor*			mNetworkComponent;
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