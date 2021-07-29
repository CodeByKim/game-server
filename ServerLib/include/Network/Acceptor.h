#pragma once
#include "../Common/CommonLib.h"
#include "./NetAddress.h"
#include "../Common/IAllacator.h"

namespace garam
{
	namespace net
	{		
		class Socket;

		class Acceptor
		{
		public:
			Acceptor();
			void RegisterAcceptCallback(std::function<void(Socket*)> callback);
			void Run();			
			void SetAlloctor(memory::IAllocator<Socket*>* allocator);
			void ReleaseSocket(Socket* sock);

		private:
			SOCKET mListenSocket;
			NetAddress mAddress;
			HANDLE mIocp;
			std::thread mAcceptThread;
			std::function<void(Socket*)> mAcceptCallback;			
			memory::IAllocator<Socket*>* mAllocator;
		};
	}
}