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
			 * 전송 버퍼 사이즈 0으로, 비동기 처리 위해
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
			 * WSARecv할 때 인자 제대로 다 안넣어주면 에러남
			 * flag를 안쓰더라도 넣어줘야 함
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
			 * error가 IO_PENDING이 아니면 끊어줌
			 * 단 여기서 무조건 close 하면 안됨
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
		 * 패킷 모아보내기를 지원해야 함
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
			 * error가 IO_PENDING이 아니면 끊어줌
			 * 단 여기서 무조건 close하면 안됨
			 */
			if (result == SOCKET_ERROR)
			{
				int error = WSAGetLastError();
				if (error != WSA_IO_PENDING)
				{					
					//TODO : io카운트를 감소하는 구간에서는 무조건 0인지 체크하고 세션 릴리즈하는 로직 필요
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