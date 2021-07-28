#include "./Network/NetService.h"
#include "./Network/Socket.h"

namespace garam
{
	namespace net
	{
		NetService::NetService()
		{
			WSAData data;
			if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
			{				
				std::cout << "ERROR WSAStartup" << std::endl;
			}

			SYSTEM_INFO info;
			GetSystemInfo(&info);
			mWorkerThreadCount = info.dwNumberOfProcessors;			

			mIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE,
										   NULL,
										   NULL,
										   mWorkerThreadCount);

			for (int i = 0; i < mWorkerThreadCount; i++)
			{
				mWorkerThreads.push_back(std::thread(&NetService::WorkerThread, this));													 
			}
		}

		NetService::~NetService()
		{
			WSACleanup();
		}

		void NetService::AssociateCompletePort(SOCKET handle, Socket* sock)
		{
			CreateIoCompletionPort((HANDLE)handle,
								   mIocp,
								   (ULONG_PTR)sock,
								   NULL);
		}

		ULONGLONG closeCount = 0;
		void NetService::WorkerThread()
		{
			while (true)
			{
				Socket* sock = nullptr;
				NetIoEvent* ioEvent = nullptr;
				DWORD bytesTransferred = 0;

				BOOL result = GetQueuedCompletionStatus(mIocp,
														&bytesTransferred,
														(PULONG_PTR)&sock,
														(LPOVERLAPPED*)&ioEvent,
														INFINITE);

				if (!result)
				{
					if (ioEvent == nullptr)
					{
						std::cout << "ERROR(ioInfo == nullptr) : " << WSAGetLastError() << std::endl;
						break;
					}
					else
					{
						int error = WSAGetLastError();

						/*
						 * ERROR_NETNAME_DELETED은 클라의 강제 종료
						 * 강제 종료는 에러로 보지 않음
						 */
						if (error != ERROR_NETNAME_DELETED)
						{
							std::cout << "ERROR(error != ERROR_NETNAME_DELETED) : " << WSAGetLastError() << std::endl;
						}
					}
				}

				/*
				 * PostQueuedCompletionStatus로 key를 NULL로 넣음
				 * 종료 처리
				 */
				if (sock == nullptr)
				{
					break;
				}

				/*
				 * 바로 끊지말고 IoCount가 0이 되는 Socket만 제거
				 * 끊긴 Socket이라도 이미 작업중인 IO가 있을 수 있음
				 */
				if (bytesTransferred == 0)
				{
					/*
					 * bytesTransferred 0은 상대방의 정상 종료
					 * Recv로 올려놓은 IoCount 감소
					 */
					InterlockedDecrement((ULONGLONG*)&sock->mIoCount);

					/*
					 * 서버가 걸어놓은 비동기 Send가 없을 때만 Socket 제거
					 */
					//if (sock->mIoCount <= 0 && !sock->mIsSending)
					if (sock->mIoCount <= 0)
					{
						/*
						 * 사실 ioCount가 0이하로 내려갈 일은 없음
						 */
						if (sock->mIoCount < 0)
							std::cout << "IOCOUNT ERROR" << std::endl;

						//ConnectionManager::GetInstance().ReleaseConnection(conn);

						sock->Close();
						std::cout << "Disconnect Client : " << InterlockedIncrement(&closeCount) << std::endl;

						continue;
					}
				}

				if (ioEvent->GetType() == eIoEventType::Receive)
				{					
					sock->mReceiveCallback(bytesTransferred);
				}
				else if (ioEvent->GetType() == eIoEventType::Send)
				{					
					sock->mSendCallback();
				}

				InterlockedDecrement((ULONGLONG*)&sock->mIoCount);
			}
		}
	}
}
