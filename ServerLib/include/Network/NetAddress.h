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
			NetAddress(std::wstring_view ip, short port);

			SOCKADDR_IN GetAddress();
			short GetPortNumber();
			std::wstring_view GetIp();
			void Clear();

		private:
			SOCKADDR_IN mAddress;
			std::wstring mIP;
			short mPort;
		};
	}
}
