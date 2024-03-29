#pragma once
#include "../Common/IAllacator.h"
#include "../Network/Socket.h"

namespace garam
{
	namespace net
	{
		class SocketAllocator : public memory::IAllocator<Socket*>
		{
		public:
			SocketAllocator()
				: mHead(nullptr)
				, mCapacity(0)
				, mUseCount(0)
			{		
			}

			Socket* Alloc() override
			{
				Node* node = nullptr;
				if (mUseCount >= mCapacity)
					node = AllocNode();
				else
					node = Pop();

				mUseCount += 1;

				/*
				 * 구조체 자체의 주소는 첫번째 멤버의 주소와 같음
				 * 즉 TestObj의 주소값 == node의 주솟값
				 * 그래서 아래처럼 캐스팅이 가능
				 */
				return (Socket*)node;
			}

			void Free(Socket* obj) override
			{				
				Node* node = (Node*)obj;

				Push(node);
				mUseCount -= 1;
			}

			int mUseCount;
			int mCapacity;

		private:
			struct Node
			{
				/*
				 * 구조체 자체의 주소는 첫번째 멤버의 주소와 같음
				 * 첫번째 멤버는 포인터가 아니라 값으로 선언되어 있어야 함
				 * 그래야 캐스팅이 됨
				 */
				Socket data;
				Node* next;

				Node()
					: next(nullptr)
				{
				}
			};

			void Push(Node* node)
			{
				node->next = mHead;
				mHead = node;
			}

			Node* Pop()
			{				
				Node* node = mHead;
				mHead = mHead->next;

				return node;
			}

			Node* AllocNode()
			{				
				/*
				 * Node 하나를 생성하는 구문이지만
				 * 내부에 같이 선언된 TestObj도 같이 생성시킴
				 * 그래서 총 8(TestObj 객체) + 8(next 포인터) = 16바이트의 크기를 잡음
				 */
				Node* node = new Node();
				mCapacity += 1;
				return node;
			}

			Node* mHead;
		};
	}
}