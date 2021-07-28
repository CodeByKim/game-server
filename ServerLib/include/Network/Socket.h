#pragma once
#include "../Common/CommonLib.h"
#include "NetIoEvent.h"
#include "NetAddress.h"

namespace garam
{
	namespace net
	{					
		class DataBuffer
		{
		public:			
			friend class Socket;

			DataBuffer()
				: buf{ 0, }
				, count(0)
			{
			}

			void Add(char* data, int length)
			{
				buf[count].buf = data;
				buf[count].len = length;

				count += 1;
			}

			int GetCount()
			{
				return count;
			}

		private:
			WSABUF buf[100];
			int count;
		};

		class Socket
		{
		public:
			friend class NetService;
		
			Socket();
			Socket(SOCKET sock, SOCKADDR_IN address);
			~Socket();

			void						Initialize(SOCKET sock, SOCKADDR_IN address);
			void						AsyncReceive(char* buffer, int bufferLength);
			void						AsyncSend(DataBuffer buffer);
			void						RegisterReceiveCallback(std::function<void(int)> callback);
			void						RegisterSendCallback(std::function<void()> callback);
			void						RegisterCloseCallback(std::function<void()> callback);
			bool						IsConnect();
			void						Close();
			short						GetPortNumber();
			std::string_view			GetIp();

		private:
			SOCKET						mHandle;
			NetAddress					mAddress;
			NetIoEvent					mRecvEvent;
			NetIoEvent					mSendEvent;			
			std::function<void(int)>	mReceiveCallback;
			std::function<void()>		mSendCallback;
			std::function<void()>		mCloseCallback;
			int							mIoCount;
		};
	}
}