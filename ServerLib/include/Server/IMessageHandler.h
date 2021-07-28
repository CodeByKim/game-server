#pragma once
#include "../Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		class NetPacket;
		class ClientInfo;

		class IMessageHandler
		{
		public:
			virtual void OnClientJoin(ClientInfo* client) = 0;
			virtual void OnClientLeave(ClientInfo* client) = 0;
			virtual void OnPacketReceive(ClientInfo* client, NetPacket* packet) = 0;
			virtual void OnUpdate() = 0;
		};
	}
}