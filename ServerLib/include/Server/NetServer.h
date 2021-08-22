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
		class NetworkComponent;

		class NetServer
		{
		public:
			friend class NetworkComponent;

			/*
			 * TODO : ���߿��� Config ������ �޵��� �����ؾ� ��
			 */
			NetServer(short port, int ccu, IMessageHandler* handler);
			~NetServer();
			
			void Run();
			//void RegisterMessageHandler(IMessageHandler* handler);			

		protected:									
			IMessageHandler* mMessageHandler;
			//TODO : ���߿� std::vector<IComponent*> mComponents �̷��� 
			NetworkComponent* mNetworkComponent;		
			
		private:					
			void InitLogger();
			virtual void OnAccept(Connection* conn);
			virtual void OnUpdate();
			virtual void OnPacketReceive(Connection* conn, NetPacket* packet);
			virtual void OnClose(Connection* conn);			
		};
	}
}
