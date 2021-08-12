using System;
using System.Collections;
using System.Collections.Generic;
using System.Threading;

public struct PacketHeader
{
    public short length;

    public PacketHeader(short length)
    {
        this.length = length;
    }

    public void Parse(byte[] bytes)
    {
        if (bytes == null || bytes.Length == 0)
            return;

        length = BitConverter.ToInt16(bytes, 0);
    }

    public byte[] GetBytes()
    {
        return BitConverter.GetBytes(length);
    }

    public static int GetHeaderSize()
    {
        return sizeof(short);
    }
};

public class NetPacket
{
    public enum PacketType
    {
        Connect,
        Disconnect,
        Send,
        Receive
    }

    private PacketHeader mHeader;

    private byte[] mBuffer;
	private int mBufferFrontIndex;
	private int mBufferRearIndex;
	private int mSize;
    private PacketType mPacketType;

    private static NetPacketAllocator mSendPacketAllocator = new NetPacketAllocator(50, PacketType.Send);
    private static NetPacketAllocator mRecvPacketAllocator = new NetPacketAllocator(50, PacketType.Receive);

    public PacketType Type
    {
        get { return mPacketType; }
        set { mPacketType = value; }
    }

    public static NetPacket Alloc()
    {
        return mSendPacketAllocator.Pop();        
    }

    public static NetPacket Alloc(byte[] buffer)
    {
        NetPacket packet = mRecvPacketAllocator.Pop();        
        packet.SetBuffer(buffer);

        return packet;
    }

    public static void Free(NetPacket packet)
    {        
        if(packet.mPacketType == PacketType.Send)
        {
            mSendPacketAllocator.Push(packet);
        }
        else
        {
            packet.Type = PacketType.Receive;
            mRecvPacketAllocator.Push(packet);
        }       
    }

    public NetPacket(PacketType type)
    {
		mBuffer = new byte[2048];
        mBufferFrontIndex = PacketHeader.GetHeaderSize();
		mBufferRearIndex = PacketHeader.GetHeaderSize();
        mSize = 0;
        mPacketType = type;
	}

    public void PutData(byte[] data)
    {
        int size = data.Length;

        if (mSize + size > 1024 - PacketHeader.GetHeaderSize())
        {                        
            return;
        }
        
        Array.Copy(data, 0, mBuffer, mBufferRearIndex, size);
        mSize += size;
        mBufferRearIndex += size;
    }

    public byte[] GetData(int size)
    {
        if (size > mSize)
        {            
            return null;
        }

        byte[] outData = new byte[size];
        
        Array.Copy(mBuffer, mBufferFrontIndex, outData, 0, size);
        mBufferFrontIndex += size;
        mSize -= size;

        return outData;
    }

    public void Clear()
    {        
        mBufferFrontIndex = PacketHeader.GetHeaderSize();
        mBufferRearIndex = PacketHeader.GetHeaderSize();
        mSize = 0;
    }

    public int GetSize()
    {
        return mSize;
    }

    public byte[] GetBuffer()
    {
        return mBuffer;
    }
    
    public void SetHeader(PacketHeader header)
    {
        mHeader = header;
        
        Array.Copy(mHeader.GetBytes(), 0, mBuffer, 0, PacketHeader.GetHeaderSize());
    }

    private void SetBuffer(byte[] buffer)
    {
        if (buffer == null)
            return;

        mBuffer = buffer;
        mSize = buffer.Length;
    }

    

    #region Push Data
    public NetPacket Push(byte value)
    {
        PutData(BitConverter.GetBytes(value));
        return this;
    }

    public NetPacket Push(bool value)
    {
        PutData(BitConverter.GetBytes(value));
        return this;
    }

    public NetPacket Push(char value)
    {
        PutData(BitConverter.GetBytes(value));
        return this;
    }

    public NetPacket Push(short value)
    {
        PutData(BitConverter.GetBytes(value));
        return this;
    }

    public NetPacket Push(int value)
    {        
        PutData(BitConverter.GetBytes(value));
        return this;
    }

    public NetPacket Push(float value)
    {
        PutData(BitConverter.GetBytes(value));
        return this;
    }
    #endregion

    #region Pop Data
    public NetPacket Pop(out byte value)
    {
        byte[] data = GetData(sizeof(byte));
        value = data[0];
        return this;
    }

    public NetPacket Pop(out bool value)
    {
        if (sizeof(bool) > mSize)
        {
            Console.WriteLine("PacketException : Pop");
            value = default(bool);
            return null;
        }

        value = BitConverter.ToBoolean(mBuffer, mBufferFrontIndex);
        mBufferFrontIndex += sizeof(bool);
        mSize -= sizeof(bool);

        return this;
    }

    public NetPacket Pop(out char value)
    {
        if (sizeof(char) > mSize)
        {
            Console.WriteLine("PacketException : Pop");
            value = default(char);
            return null;
        }

        value = BitConverter.ToChar(mBuffer, mBufferFrontIndex);
        mBufferFrontIndex += sizeof(char);
        mSize -= sizeof(char);

        return this;
    }

    public NetPacket Pop(out short value)
    {
        if (sizeof(short) > mSize)
        {
            Console.WriteLine("PacketException : Pop");
            value = default(short);
            return null;
        }

        value = BitConverter.ToInt16(mBuffer, mBufferFrontIndex);
        mBufferFrontIndex += sizeof(short);
        mSize -= sizeof(short);

        return this;
    }

    public NetPacket Pop(out int value)
    {
        if (sizeof(int) > mSize)
        {            
            Console.WriteLine("PacketException : Pop");
            value = default(int);
            return null;
        }

        value = BitConverter.ToInt32(mBuffer, mBufferFrontIndex);
        mBufferFrontIndex += sizeof(int);
        mSize -= sizeof(int);

        return this;
    }

    public NetPacket Pop(out float value)
    {
        if (sizeof(float) > mSize)
        {
            Console.WriteLine("PacketException : Pop");
            value = default(float);
            return null;
        }
        
        value = BitConverter.ToSingle(mBuffer, mBufferFrontIndex);
        mBufferFrontIndex += sizeof(float);
        mSize -= sizeof(float);
        
        return this;
    }
    #endregion
}

public class NetPacketAllocator
{
    private Stack<NetPacket> mPool;
    private Object mLock;
    private int mAllocCount;
    private int mCapacity;
    private NetPacket.PacketType mType;
    
    public NetPacketAllocator(int capacity, NetPacket.PacketType type)
    {
        mPool = new Stack<NetPacket>();
        mLock = new Object();
        mType = type;

        for(int i = 0; i < capacity; i++)
        {
            mPool.Push(new NetPacket(type));
        }
    }

    public void Push(NetPacket packet)
    {
        packet.Clear();

        lock (mLock)
        {
            mAllocCount -= 1;
            mPool.Push(packet);
        }        
    }

    public NetPacket Pop()
    {        
        lock (mLock)
        {
            mAllocCount += 1;

            if (mPool.Count == 0)
            {
                mCapacity += 1;
                return new NetPacket(mType);
            }

            return mPool.Pop();
        }            
    }

    public int GetAllocCount()
    {
        return mAllocCount;
    }
}