#pragma once
#include "../Common/CommonLib.h"
#include "NetPacketAllocator.h"

namespace garam
{
	namespace net
	{				
		struct PacketHeader
		{
			short length;
		};

		class PacketException : public std::exception
		{
		public:
			enum class ePacketOperationType
			{
				GetData,
				PutData
			};

		public:
			PacketException(ePacketOperationType type, int packetSize, int trySize);
			~PacketException();

			char const* what() const override;
			std::wstring Log();

		private:
			ePacketOperationType mType;
			int mPacketSize;
			int mTrySize;
		};
		
		class NetPacket
		{
		public:
			friend class Connection;
			friend class NetPacketAllocator;

			NetPacket();
			~NetPacket();

			void PutData(char* data, int size);
			void GetData(char* outData, int size);
			void Clear();
			int GetSize();

			void AddRefCount();
			void Release();
			PacketHeader GetHeader();
			void SetHeader(short length);
			char* GetBuffer();

			static NetPacket* Alloc();
			static void Free(NetPacket* packet);

			/*
			 * 데이터 삽입
			 */
			NetPacket& operator<< (BYTE& data);
			NetPacket& operator<< (bool& data);
			NetPacket& operator<< (char& data);
			NetPacket& operator<< (WCHAR& data);
			NetPacket& operator<< (short& data);
			NetPacket& operator<< (int& data);
			NetPacket& operator<< (__int64& data);
			NetPacket& operator<< (float& data);
			NetPacket& operator<< (double& data);

			/*
			 * 데이터 가져오기
			 */
			NetPacket& operator>> (BYTE& outData);
			NetPacket& operator>> (bool& outData);
			NetPacket& operator>> (char& outData);
			NetPacket& operator>> (WCHAR& outData);
			NetPacket& operator>> (short& outData);
			NetPacket& operator>> (int& outData);
			NetPacket& operator>> (__int64& data);
			NetPacket& operator>> (float& outData);
			NetPacket& operator>> (double& outData);			

		private:						
			char mBuffer[2048];			
			char* mBufferFront;
			char* mBufferRear;
			int mSize;
			int mRefCount;

			static NetPacketAllocator mAllocator;
		};
	}	
}