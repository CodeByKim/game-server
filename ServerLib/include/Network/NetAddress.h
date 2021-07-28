#pragma once
#include "../Common/CommonLib.h"

namespace garam
{
	namespace net
	{
		class NetAddress
		{
		public:
			NetAddress();
			NetAddress(SOCKADDR_IN address);
			NetAddress(std::string_view ip, short port);

			SOCKADDR_IN GetAddress();
			short GetPortNumber();
			std::string_view GetIp();
			void Clear();

		private:
			SOCKADDR_IN mAddress;
			std::string mIP;
			short mPort;
		};
	}
}
