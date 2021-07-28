#include "./Network/NetAddress.h"



namespace garam
{
	namespace net
	{
		NetAddress::NetAddress()
			: mIP("")
			, mAddress {0,}
			, mPort(0)
		{
		}

		NetAddress::NetAddress(SOCKADDR_IN address)			
			: mAddress(address)
			, mPort(address.sin_port)
		{	
			char tempBuf[INET_ADDRSTRLEN];
			inet_ntop(AF_INET, &address.sin_addr, tempBuf, INET_ADDRSTRLEN);
			mIP = tempBuf;
		}

		NetAddress::NetAddress(std::string_view ip, short port)
			: mIP(ip)
			, mAddress{ 0, }
			, mPort(port)
		{			
			mAddress.sin_family = AF_INET;
			mAddress.sin_port = htons(mPort);
			inet_pton(AF_INET, mIP.data(), &mAddress.sin_addr.S_un.S_addr);
		}

		SOCKADDR_IN NetAddress::GetAddress()
		{
			return mAddress;
		}

		short NetAddress::GetPortNumber()
		{
			return mPort;
		}

		std::string_view NetAddress::GetIp()
		{
			return mIP;
		}

		void NetAddress::Clear()
		{
			mIP = "";
			ZeroMemory(&mAddress, sizeof(SOCKADDR_IN));			
			mPort = 0;				
		}
	}
}