using System;
using System.Collections;
using System.Collections.Generic;

public class RingBuffer
{
	private byte[] mBuffer;
	private int mBufferFrontIndex;
	private int mBufferRearIndex;
	private int mBufferEndIndex;
	private int mSize;
	private int mCapacity;

	public RingBuffer(int capacity)
    {
		mCapacity = capacity;
		mSize = 0;

		mBuffer = new byte[mCapacity];
		mBufferFrontIndex = 0;
		mBufferRearIndex = 0;
		mBufferEndIndex = mCapacity - 1;
	}

	public bool Enqueue(byte[] data, int size)
    {		
		if (size > GetFreeSize())
		{
			return false;
		}

		if (size <= (mBufferEndIndex - mBufferRearIndex - 1))
		{			
			Array.Copy(data, 0, mBuffer, mBufferRearIndex, size);
			mBufferRearIndex += size;
		}
		else
		{
			int tempSize = mBufferEndIndex - mBufferRearIndex;			
			Array.Copy(data, 0, mBuffer, mBufferRearIndex, tempSize);
			mBufferRearIndex = 0;
			
			Array.Copy(data, tempSize, mBuffer, mBufferRearIndex, size - tempSize);
			mBufferRearIndex += (size - tempSize);
		}

		mSize += size;
		return true;
	}

	public byte[] Dequeue(int size)
    {		
		if (size > mSize)
		{
			return null;
		}

		byte[] outData = new byte[size];

		if (size <= (mBufferEndIndex - mBufferFrontIndex - 1))
		{			
			Array.Copy(mBuffer, mBufferFrontIndex, outData, 0, size);			
			mBufferFrontIndex += size;
		}
		else
		{						
			int tempSize = mBufferEndIndex - mBufferFrontIndex;			
			Array.Copy(mBuffer, mBufferFrontIndex, outData, 0, tempSize);
			mBufferFrontIndex = 0;
			
			Array.Copy(mBuffer, mBufferFrontIndex, outData, tempSize, size - tempSize);
			mBufferFrontIndex += (size - tempSize);
		}

		mSize -= size;
		return outData;
	}

	public byte[] Peek(int size)
    {
		if (size > mSize)
		{
			return null;
		}

		byte[] outData = new byte[size];

		if (size <= (mBufferEndIndex - mBufferFrontIndex - 1))
		{			
			Array.Copy(mBuffer, mBufferFrontIndex, outData, 0, size);
		}
		else
		{			
			int tempSize = mBufferEndIndex - mBufferFrontIndex;			
			Array.Copy(mBuffer, mBufferFrontIndex, outData, 0, tempSize);			
			Array.Copy(mBuffer, 0, outData, tempSize, size - tempSize);
		}

		return outData;
	}

	public int GetFreeSize()
    {
		return mCapacity - mSize - 1;
	}

	public bool IsEmpty()
	{
		return mSize == 0;
	}

	public int GetUseSize()
	{
		return mSize;
	}

	public void Clear()
	{
		mBufferFrontIndex = 0;
		mBufferRearIndex = 0;
		mBufferEndIndex = mCapacity - 1;
		mSize = 0;
	}
}
