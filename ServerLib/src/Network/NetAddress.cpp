#include "./Network/NetAddress.h"



namespace garam
{
	namespace net
	{
		NetAddress::NetAddress()
			: mIP(L"")
			, mAddress {0,}
			, mPort(0)
		{
		}

		NetAddress::NetAddress(SOCKADDR_IN address)			
			: mAddress(address)
			, mPort(address.sin_port)
		{	
			WCHAR tempBuf[INET_ADDRSTRLEN];			
			InetNtop(AF_INET, &address.sin_addr, tempBuf, INET_ADDRSTRLEN);
			mIP = tempBuf;			
		}

		NetAddress::NetAddress(std::wstring_view ip, short port)
			: mIP(ip)
			, mAddress{ 0, }
			, mPort(port)
		{						
			mAddress.sin_family = AF_INET;
			mAddress.sin_port = htons(mPort);			
			InetPton(AF_INET, mIP.data(), &mAddress.sin_addr.S_un.S_addr);
		}

		SOCKADDR_IN NetAddress::GetAddress()
		{
			return mAddress;
		}

		short NetAddress::GetPortNumber()
		{
			return mPort;
		}

		std::wstring_view NetAddress::GetIp()
		{
			return mIP;
		}

		void NetAddress::Clear()
		{
			mIP = L"";
			ZeroMemory(&mAddress, sizeof(SOCKADDR_IN));			
			mPort = 0;				
		}
	}
}