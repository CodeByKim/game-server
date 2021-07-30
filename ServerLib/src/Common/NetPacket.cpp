#include "./Common/NetPacket.h"

namespace garam
{
    namespace net
    {
        PacketException::PacketException(ePacketOperationType type, int packetSize, int trySize)
            : mType(type)
            , mPacketSize(packetSize)
            , mTrySize(trySize)
        {
        }

        PacketException::~PacketException()
        {
        }

        char const* PacketException::what() const
        {
            return "Packet Exception";
        }

        std::wstring PacketException::Log()
        {
            std::wostringstream os;
            os << "[PacketException] =>";
            os << " Type : " << ((mType == ePacketOperationType::GetData) ? "GutData" : "PutData");
            os << ", PacketSize : " << mPacketSize;
            os << ", TrySize : " << mTrySize;

            return os.str();
        }

        NetPacket::NetPacket()
            : mBuffer{ 0, }
            , mBufferFront(mBuffer + sizeof(PacketHeader))
            , mBufferRear(mBuffer + sizeof(PacketHeader))            
            , mSize(0)
            , mRefCount(1)
        {            
        }

        NetPacket::~NetPacket()
        {
        }

        void NetPacket::PutData(char* data, int size)
        {
            /*
             * 이거 버그 가능성 있는데..
             * 이렇게 수정하자..
             */
            if (mSize + size > 1024 - sizeof(PacketHeader))
            {
                throw PacketException(PacketException::ePacketOperationType::PutData, mSize, size);
            }

            CopyMemory(mBufferRear, data, size);
            mSize += size;
            mBufferRear += size;
        }

        void NetPacket::GetData(char* outData, int size)
        {
            if (size > mSize)
            {
                throw PacketException(PacketException::ePacketOperationType::GetData, mSize, size);
            }

            CopyMemory(outData, mBufferFront, size);
            mBufferFront += size;
            mSize -= size;
        }

        void NetPacket::Clear()
        {
            mBufferFront = mBuffer + sizeof(PacketHeader);
            mBufferRear = mBuffer + sizeof(PacketHeader);
            mSize = 0;
        }

        int NetPacket::GetSize()
        {
            return mSize;
        }

        void NetPacket::AddRefCount()
        {
            InterlockedIncrement((UINT*)&mRefCount);
        }

        void NetPacket::Release()
        {
            if (InterlockedDecrement((UINT*)&mRefCount) == 0)
            {                
                delete this;                
            }
        }

        PacketHeader NetPacket::GetHeader()
        {
            PacketHeader header;
            CopyMemory(&header, mBuffer, sizeof(PacketHeader));

            return header;
        }

        void NetPacket::SetHeader(short length)
        {
            PacketHeader header;
            header.length = length;

            CopyMemory(mBuffer, &header, sizeof(PacketHeader));
        }

        char* NetPacket::GetBuffer()
        {
            return mBuffer;
        }

#pragma region operator>> (Put Data)
        NetPacket& NetPacket::operator<<(BYTE& data)
        {
            PutData((char*)&data, sizeof(BYTE));
            return *this;
        }

        NetPacket& NetPacket::operator<<(bool& data)
        {
            PutData((char*)&data, sizeof(bool));
            return *this;
        }

        NetPacket& NetPacket::operator<<(char& data)
        {
            PutData((char*)&data, sizeof(char));
            return *this;
        }

        NetPacket& NetPacket::operator<<(WCHAR& data)
        {
            PutData((char*)&data, sizeof(WCHAR));
            return *this;
        }

        NetPacket& NetPacket::operator<<(short& data)
        {
            PutData((char*)&data, sizeof(short));
            return *this;
        }

        NetPacket& NetPacket::operator<<(int& data)
        {
            PutData((char*)&data, sizeof(int));
            return *this;
        }

        NetPacket& NetPacket::operator<<(__int64& data)
        {
            PutData((char*)&data, sizeof(__int64));
            return *this;
        }

        NetPacket& NetPacket::operator<<(float& data)
        {
            PutData((char*)&data, sizeof(float));
            return *this;
        }

        NetPacket& NetPacket::operator<<(double& data)
        {
            PutData((char*)&data, sizeof(double));
            return *this;
        }
#pragma endregion

#pragma region operator<< (Get Data)
        NetPacket& NetPacket::operator>>(BYTE& outData)
        {
            GetData((char*)&outData, sizeof(BYTE));
            return *this;
        }

        NetPacket& NetPacket::operator>>(bool& outData)
        {
            GetData((char*)&outData, sizeof(bool));
            return *this;
        }

        NetPacket& NetPacket::operator>>(char& outData)
        {
            GetData((char*)&outData, sizeof(char));
            return *this;
        }

        NetPacket& NetPacket::operator>>(WCHAR& outData)
        {
            GetData((char*)&outData, sizeof(WCHAR));
            return *this;
        }

        NetPacket& NetPacket::operator>>(short& outData)
        {
            GetData((char*)&outData, sizeof(short));
            return *this;
        }

        NetPacket& NetPacket::operator>>(int& outData)
        {
            GetData((char*)&outData, sizeof(int));
            return *this;
        }

        NetPacket& NetPacket::operator>>(__int64& outData)
        {
            GetData((char*)&outData, sizeof(__int64));
            return *this;
        }

        NetPacket& NetPacket::operator>>(float& outData)
        {
            GetData((char*)&outData, sizeof(float));
            return *this;
        }

        NetPacket& NetPacket::operator>>(double& outData)
        {
            GetData((char*)&outData, sizeof(double));
            return *this;
        }
#pragma endregion

        NetPacket* PacketAllocator::Alloc()
        {
            return mPool.Alloc();              
        }

        void PacketAllocator::Free(NetPacket* packet)
        {               
            if (InterlockedDecrement((UINT*)&packet->mRefCount) == 0)
            {
                mPool.Free(packet);
            }
        }
        
        PacketAllocator& PacketAllocator::GetInstance()
        {            
            static PacketAllocator instance;
            return instance;
        }

        PacketAllocator::PacketAllocator()            
        {
        }
    }
}

