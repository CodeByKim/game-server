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
			 * TODO : ���߿��� Config ������ �޵��� �����ؾ� ��
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
