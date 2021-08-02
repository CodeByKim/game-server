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

			/*
			 * TODO : ���߿��� Config ������ �޵��� �����ؾ� ��
			 */
			NetServer(short port, int ccu);
			~NetServer();
			
			void Run();
			void RegisterMessageHandler(IMessageHandler* handler);			

		protected:									
			IMessageHandler* mMessageHandler;
			Acceptor mAcceptor;
			ConnectionManager mConnectionManager;

		private:					
			void InitLogger();
			virtual void OnAccept(Socket* sock);
			virtual void OnUpdate();
			virtual void OnPacketReceive(Connection* conn, NetPacket* packet);
			virtual void OnClose(Connection* conn);			
		};
	}
}
