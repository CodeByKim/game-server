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
				/*int count = mCapacity;

				for (int i = 0; i < count; i++)
				{
					Node* node = AllocNode();
					Push(node);
				}*/				
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
				 * ����ü ��ü�� �ּҴ� ù��° ����� �ּҿ� ����
				 * �� TestObj�� �ּҰ� == node�� �ּڰ�
				 * �׷��� �Ʒ�ó�� ĳ������ ����
				 */
				return (Socket*)node;
			}

			void Free(Socket* obj) override
			{
				std::cout << "Free SOCKET" << std::endl;

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
				 * ����ü ��ü�� �ּҴ� ù��° ����� �ּҿ� ����
				 * ù��° ����� �����Ͱ� �ƴ϶� ������ ����Ǿ� �־�� ��
				 * �׷��� ĳ������ ��
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
				std::cout << "POP SOCKET" << std::endl;

				Node* node = mHead;
				mHead = mHead->next;

				return node;
			}

			Node* AllocNode()
			{
				std::cout << "ALLOC NEW SOCKET" << std::endl;
				/*
				 * Node �ϳ��� �����ϴ� ����������
				 * ���ο� ���� ����� TestObj�� ���� ������Ŵ
				 * �׷��� �� 8(TestObj ��ü) + 8(next ������) = 16����Ʈ�� ũ�⸦ ����
				 */
				Node* node = new Node();
				mCapacity += 1;
				return node;
			}

			Node* mHead;
		};
	}
}