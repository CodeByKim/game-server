#pragma once
#include "../Common/CommonLib.h"
#include "../Network/Acceptor.h"
#include "./ConnectionManager.h"

namespace garam
{
	namespace net
	{
		class Connection;
		class NetPacket;
		class IMessageHandler;
		class ClientInfo;
		class NetworkSensor;

		class NetServer
		{
		public:
			friend class NetworkSensor;

			/*
			 * TODO : 나중에는 Config 파일을 받도록 수정해야 함
			 */
			NetServer(short port, int ccu, IMessageHandler* handler);
			~NetServer();
			
			void Run();			

		protected:									
			IMessageHandler* mMessageHandler;		
			NetworkSensor* mNetworkSensor;		
			
		private:					
			void InitLogger();
			virtual void OnAccept(Connection* conn);
			virtual void OnUpdate();
			virtual void OnPacketReceive(Connection* conn, NetPacket* packet);
			virtual void OnClose(Connection* conn);			
		};
	}
}
