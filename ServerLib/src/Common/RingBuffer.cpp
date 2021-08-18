#include "./Common/RingBuffer.h"

namespace garam
{
	namespace net
	{
		/*
 * 버퍼의 마지막 끝칸을 비우는게 Rear, End 계산하기가 편함
 * 마지막 칸을 비워야 Rear가 버퍼 밖을 안넘어가게 막을 수 있음
 */
		RingBuffer::RingBuffer(int size)
			: mBuffer(nullptr)
			, mBufferFront(nullptr)
			, mBufferRear(nullptr)
			, mBufferEnd(nullptr)
			, mCapacity(size)
			, mSize(0)
		{
			mBuffer = new char[mCapacity];
			mBufferFront = mBuffer;
			mBufferRear = mBuffer;
			mBufferEnd = mBuffer + mCapacity - 1;
		}

		RingBuffer::~RingBuffer()
		{

		}

		bool RingBuffer::Enqueue(char* data, int size)
		{
			if (size > GetFreeSize())
			{
				return false;
			}

			// 넣으려는 사이즈가 버퍼의 남은 공간보다 작을 때
			// 이때는 데이터가 한번에 들어간다.
			// 애초에 이렇게 계산하는게 GetNextPos와 비슷한 역할을 하는거 같은데...
			if (size <= (mBufferEnd - mBufferRear - 1))
			{
				CopyMemory(mBufferRear, data, size);
				mBufferRear += size;
			}
			else
			{
				__int64 tempSize = mBufferEnd - mBufferRear;
				CopyMemory(mBufferRear, data, tempSize);
				mBufferRear = mBuffer;

				CopyMemory(mBufferRear, data + tempSize, size - tempSize);
				mBufferRear += (size - tempSize);
			}

			mSize += size;
			return true;
		}

		bool RingBuffer::Dequeue(char* outData, int size)
		{
			if (size > mSize)
			{
				return false;
			}

			//데이터를 한번에 가져올 수 있는경우
			//일단 End앞에 데이터가 다 있으면 된다. 그럼 한번에 가져올 수 있음
			if (size <= (mBufferEnd - mBufferFront - 1))
			{
				CopyMemory(outData, mBufferFront, size);
				mBufferFront += size;
			}
			else
			{
				//가져올 데이터가 애매하게 end와 front에 걸친경우
				//우선 end까지 읽고
				__int64 tempSize = mBufferEnd - mBufferFront;
				CopyMemory(outData, mBufferFront, tempSize);
				mBufferFront = mBuffer;

				CopyMemory(outData + tempSize, mBufferFront, size - tempSize);
				mBufferFront += (size - tempSize);
			}

			mSize -= size;
			return true;
		}

		bool RingBuffer::Peek(char* outData, int size)
		{
			if (size > mSize)
			{
				return false;
			}

			//데이터를 한번에 가져올 수 있는경우
			//일단 End앞에 데이터가 다 있으면 된다. 그럼 한번에 가져올 수 있음
			if (size <= (mBufferEnd - mBufferFront - 1))
			{
				CopyMemory(outData, mBufferFront, size);
			}
			else
			{
				//가져올 데이터가 애매하게 end와 front에 걸친경우
				//우선 end까지 읽고
				__int64 tempSize = mBufferEnd - mBufferFront;
				CopyMemory(outData, mBufferFront, tempSize);
				CopyMemory(outData + tempSize, mBuffer, size - tempSize);
			}

			return true;
		}

		void RingBuffer::MoveFront(int size)
		{
			//이거 rear에서 front를 빼야하는거 아니야?
			if (size <= (mBufferEnd - mBufferFront - 1))
			{
				mBufferFront += size;
			}
			else
			{
				__int64 tempSize = mBufferEnd - mBufferFront;

				mBufferFront = mBuffer;
				mBufferFront += (size - tempSize);
			}

			mSize -= size;
		}

		void RingBuffer::MoveRear(int size)
		{
			// 넣으려는 사이즈가 버퍼의 남은 공간보다 작을 때
			// 이때는 데이터가 한번에 들어간다.
			if (size <= (mBufferEnd - mBufferRear - 1))
			{
				mBufferRear += size;
			}
			else
			{
				__int64 tempSize = mBufferEnd - mBufferRear;
				mBufferRear = mBuffer;
				mBufferRear += (size - tempSize);
			}

			mSize += size;
		}

		bool RingBuffer::IsEmpty()
		{
			return mSize == 0;
		}

		int RingBuffer::GetFreeSize()
		{
			return mCapacity - mSize - 1;
		}

		int RingBuffer::GetUseSize()
		{
			return mSize;
		}

		char* RingBuffer::GetBufferFront()
		{
			return mBufferFront;
		}

		char* RingBuffer::GetBufferRear()
		{
			return mBufferRear;
		}

		int RingBuffer::GetDirectEnqueueSize()
		{
			//한번에 쓸 수 있는 사이즈
			//그럼 얘도.. rear가 front보다 앞에 있는 상황이 있다는거 아닌가?

			if (mBufferFront <= mBufferRear)
			{
				//일반적인 케이스 (체크 완료)
				return (int)(mBufferEnd - mBufferRear);
			}
			else
				return (int)(mBufferFront - mBufferRear);

		}

		//체크 완료
		int RingBuffer::GetDirectDequeueSize()
		{
			if (mBufferFront <= mBufferRear)
			{
				//일반적인 케이스 (체크 완료)
				//큐가 꽉 차있으면 front와 rear가 같은 위치에 존재..
				//그럼 0이 나올텐데...
				return (int)(mBufferRear - mBufferFront);
			}
			else
				return (int)(mBufferEnd - mBufferFront);
		}

		void RingBuffer::Clear()
		{
			mBufferFront = mBuffer;
			mBufferRear = mBuffer;
			mBufferEnd = mBuffer + mCapacity - 1;

			mSize = 0;
		}

		void RingBuffer::Swap(RingBuffer& a, RingBuffer& b)
		{
			char* buffer = a.mBuffer;
			char* front = a.mBufferFront;
			char* rear = a.mBufferRear;
			char* end = a.mBufferEnd;
			int size = a.mSize;
			int capacity = a.mCapacity;

			a.mBuffer = b.mBuffer;
			a.mBufferFront = b.mBufferFront;
			a.mBufferRear = b.mBufferRear;
			a.mBufferEnd = b.mBufferEnd;
			a.mSize = b.mSize;
			a.mCapacity = b.mCapacity;

			b.mBuffer = buffer;
			b.mBufferFront = front;
			b.mBufferRear = rear;
			b.mBufferEnd = end;
			b.mSize = size;
			b.mCapacity = capacity;
		}
	}
}