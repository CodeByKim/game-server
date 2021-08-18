#pragma once
#include <Windows.h>

namespace garam
{
	namespace net
	{
		class RingBuffer
		{
		public:
			RingBuffer(int size);
			~RingBuffer();

			bool Enqueue(char* data, int size);
			bool Dequeue(char* outData, int size);
			bool Peek(char* outData, int size);
			void MoveFront(int size);
			void MoveRear(int size);
			bool IsEmpty();
			int GetFreeSize();
			int GetUseSize();
			char* GetBufferFront();
			char* GetBufferRear();
			int GetDirectEnqueueSize();
			int GetDirectDequeueSize();
			void Clear();

			static void Swap(RingBuffer& a, RingBuffer& b);

		private:
			char* mBuffer;
			char* mBufferFront;
			char* mBufferRear;
			char* mBufferEnd;
			int mSize;
			int mCapacity;
		};
	}
}