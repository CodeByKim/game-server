#include "./Network/Acceptor.h"
#include "./Network/Socket.h"

namespace garam
{
	namespace net
	{		
		Acceptor::Acceptor()
			: mListenSocket(INVALID_SOCKET)
			, mAddress("127.0.0.1", 6000)
			, mIocp(INVALID_HANDLE_VALUE)
		{
			mListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

			SOCKADDR_IN addr = mAddress.GetAddress();
			bind(mListenSocket, (SOCKADDR*)&addr, sizeof(SOCKADDR));			
			listen(mListenSocket,SOMAXCONN);			

			mAcceptThread = std::thread(&Acceptor::Run, this);
		}

		void Acceptor::RegisterAcceptCallback(std::function<void(Socket*)> callback)
		{
			mAcceptCallback = callback;			
		}

		void Acceptor::Run()
		{			
			while (true)
			{
				SOCKADDR_IN clientAddr;
				int len = sizeof(SOCKADDR_IN);

				SOCKET handle = accept(mListenSocket, (SOCKADDR*)&clientAddr, &len);		

				/*
				 * ���� ó���� �� ���������� close
				 * accept�� INVALID_SOCKET�� �����ϸ鼭 �� ��������
				 */
				if (handle == INVALID_SOCKET)
				{
					break;
				}
				
				Socket* sock = nullptr;
				if (mAllocator != nullptr)
				{
					sock = mAllocator->Alloc();
					sock->Initialize(handle, clientAddr);
				}
				else
				{
					sock = new Socket(handle, clientAddr);
				}
				
				mAcceptCallback(sock);
			}
		}

		void Acceptor::SetAlloctor(IAllocator<Socket*>* allocator)
		{
			mAllocator = allocator;
		}

		void Acceptor::ReleaseSocket(Socket* sock)
		{
			if (mAllocator != nullptr)
			{
				mAllocator->Free(sock);
			}
			else
			{
				delete sock;
			}
		}
	}
}
