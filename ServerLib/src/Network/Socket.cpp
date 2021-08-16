#include "./Network/Socket.h"
#include "./Network/NetService.h"

namespace garam
{
	namespace net
	{
		NetService gService;

		Socket::Socket()
			: mHandle(INVALID_SOCKET)
			, mRecvEvent(eIoEventType::Receive)
			, mSendEvent(eIoEventType::Send)
			, mReceiveCallback(nullptr)
			, mSendCallback(nullptr)
			, mIoCount(0)
		{			
		}

		Socket::Socket(SOCKET sock, SOCKADDR_IN address)
			: mHandle(sock)
			, mAddress(address)
			, mRecvEvent(eIoEventType::Receive)
			, mSendEvent(eIoEventType::Send)
			, mReceiveCallback(nullptr)
			, mSendCallback(nullptr)
			, mIoCount(0)
		{
			/*
			 * ���� ���� ������ 0����, �񵿱� ó�� ����
			 */
			int sendbufferSize = 0;
			setsockopt(sock,
					   SOL_SOCKET,
					   SO_SNDBUF,
					   (char*)&sendbufferSize,
					   sizeof(int));

			gService.AssociateCompletePort(sock, this);
		}

		Socket::~Socket()
		{
		}

		void Socket::Initialize(SOCKET sock, SOCKADDR_IN address)
		{
			mHandle = sock;
			mAddress = address;

			int sendbufferSize = 0;
			setsockopt(sock,
				SOL_SOCKET,
				SO_SNDBUF,
				(char*)&sendbufferSize,
				sizeof(int));

			gService.AssociateCompletePort(sock, this);
		}

		void Socket::AsyncReceive(char* buffer, int bufferLength)
		{			
			WSABUF buf;
			buf.buf = buffer;
			buf.len = bufferLength;
			mRecvEvent.Initialize();			
			InterlockedIncrement((ULONGLONG*)&mIoCount);

			/*
			 * WSARecv�� �� ���� ����� �� �ȳ־��ָ� ������
			 * flag�� �Ⱦ����� �־���� ��
			 */
			DWORD flag = 0;
			int result = WSARecv(mHandle,
								 &buf,
								 1,
								 NULL,
								 &flag,
								 (OVERLAPPED*)&mRecvEvent,
								 NULL);

			/*
			 * error�� IO_PENDING�� �ƴϸ� ������
			 * �� ���⼭ ������ close �ϸ� �ȵ�
			 */
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error != WSA_IO_PENDING)
				{
					InterlockedDecrement((ULONGLONG*)&mIoCount);
					std::cout << "[WSARecv] error != WSA_IO_PENDING : " << error << std::endl;
					return;
				}
			}
		}

		/*
		 * ��Ŷ ��ƺ����⸦ �����ؾ� ��
		 */		
		void Socket::AsyncSend(DataBuffer& buffer)
		{										
			mSendEvent.Initialize();
			InterlockedIncrement((ULONGLONG*)&mIoCount);

			int result = WSASend(mHandle,
								 buffer.buf,
								 buffer.count,
								 NULL,
								 0,
								 (OVERLAPPED*)&mSendEvent,
								 NULL);			

			/*
			 * error�� IO_PENDING�� �ƴϸ� ������
			 * �� ���⼭ ������ close�ϸ� �ȵ�
			 */
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error != WSA_IO_PENDING)
				{					
					//TODO : ioī��Ʈ�� �����ϴ� ���������� ������ 0���� üũ�ϰ� ���� �������ϴ� ���� �ʿ�
					InterlockedDecrement((ULONGLONG*)&mIoCount);			
					std::cout << "[WSASend] error != WSA_IO_PENDING : " << error << std::endl;
					return;
				}				
			}
		}

		void Socket::RegisterReceiveCallback(std::function<void(int)> callback)
		{
			mReceiveCallback = callback;
		}

		void Socket::RegisterSendCallback(std::function<void()> callback)
		{
			mSendCallback = callback;
		}

		void Socket::RegisterCloseCallback(std::function<void()> callback)
		{
			mCloseCallback = callback;
		}

		bool Socket::IsConnect()
		{
			return mHandle != INVALID_SOCKET;
		}

		void Socket::Close()
		{
			closesocket(mHandle);
			mAddress.Clear();
						
			mCloseCallback();
		}

		short Socket::GetPortNumber()
		{
			return mAddress.GetPortNumber();
		}

		std::wstring_view Socket::GetIp()
		{
			return mAddress.GetIp();
		}
	}
}