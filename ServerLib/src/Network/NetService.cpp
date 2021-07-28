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
						 * ERROR_NETNAME_DELETED�� Ŭ���� ���� ����
						 * ���� ����� ������ ���� ����
						 */
						if (error != ERROR_NETNAME_DELETED)
						{
							std::cout << "ERROR(error != ERROR_NETNAME_DELETED) : " << WSAGetLastError() << std::endl;
						}
					}
				}

				/*
				 * PostQueuedCompletionStatus�� key�� NULL�� ����
				 * ���� ó��
				 */
				if (sock == nullptr)
				{
					break;
				}

				/*
				 * �ٷ� �������� IoCount�� 0�� �Ǵ� Socket�� ����
				 * ���� Socket�̶� �̹� �۾����� IO�� ���� �� ����
				 */
				if (bytesTransferred == 0)
				{
					/*
					 * bytesTransferred 0�� ������ ���� ����
					 * Recv�� �÷����� IoCount ����
					 */
					InterlockedDecrement((ULONGLONG*)&sock->mIoCount);

					/*
					 * ������ �ɾ���� �񵿱� Send�� ���� ���� Socket ����
					 */
					//if (sock->mIoCount <= 0 && !sock->mIsSending)
					if (sock->mIoCount <= 0)
					{
						/*
						 * ��� ioCount�� 0���Ϸ� ������ ���� ����
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
