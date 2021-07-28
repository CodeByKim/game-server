#pragma once

namespace garam
{
	namespace net
	{
		template <typename T>
		class MemoryPool
		{
		public:
			MemoryPool()
				: mHead(nullptr)
				, mCapacity(0)
				, mUseCount(0)
			{				
				for (int i = 0; i < 1000; i++)
				{
					Node* node = AllocNode();
					Push(node);
				}
			}

			T* Alloc()
			{
				std::scoped_lock<std::mutex> lock(mLock);

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
				new (&node->data) T;
				return (T*)node;
			}

			void Free(T* obj)
			{
				std::scoped_lock<std::mutex> lock(mLock);

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
				T data;
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
				 * Node �ϳ��� �����ϴ� ����������
				 * ���ο� ���� ����� TestObj�� ���� ������Ŵ
				 * �׷��� �� 8(TestObj ��ü) + 8(next ������) = 16����Ʈ�� ũ�⸦ ����
				 */
				Node* node = new Node();
				mCapacity += 1;
				return node;
			}

			std::mutex mLock;
			Node* mHead;
		};
	}
}
