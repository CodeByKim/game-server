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
			 * TODO : 나중에는 Config 파일을 받도록 수정해야 함
			 */
			NetServer(short port, int ccu);
			~NetServer();
			
			void Run();
			//void SendPacket(int id, NetPacket* packet);
			void RegisterMessageHandler(IMessageHandler* handler);			

		protected:									
			IMessageHandler* mMessageHandler;

		private:					
			void InitLogger();
			void OnAccept(Socket* sock);
			virtual void OnUpdate();
			virtual void OnPacketReceive(Connection* conn, NetPacket* packet);
			virtual void OnClose(Connection* conn);			

			Acceptor mAcceptor;
			ConnectionManager mConnectionManager;			
		};
	}
}
