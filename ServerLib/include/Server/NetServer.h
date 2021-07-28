#pragma once
#include "../Common/CommonLib.h"
#include "../Network/Acceptor.h"
#include "ConnectionManager.h"

namespace garam
{
	namespace net
	{
		class Connection;
		class NetPacket;
		class IMessageHandler;

		class NetServer
		{
		public:
			friend class Connection;
			~NetServer();
			
			void Run();
			//void SendPacket(int id, NetPacket* packet);
			void RegisterMessageHandler(IMessageHandler* handler);

			/*
			 * TODO : ���߿��� Config ������ �޵��� �����ؾ� ��
			 */
			static NetServer* Create(short port, int ccu);

		protected:						
			NetServer(short port, int ccu);

			IMessageHandler* mMessageHandler;

		private:						
			void OnAccept(Socket* sock);
			virtual void OnUpdate();
			virtual void OnPacketReceive(Connection* conn, NetPacket* packet);
			virtual void OnClose(Connection* conn);			

			Acceptor mAcceptor;
			ConnectionManager mConnectionManager;			
		};
	}
}
