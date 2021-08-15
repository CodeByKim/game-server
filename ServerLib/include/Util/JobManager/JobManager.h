#pragma once
#include "./Common/CommonLib.h"

#define JOB_INFINITY -1

namespace garam
{
	namespace jobsystem
	{
		/*
		 * IJob�� ��ӹް� �����Լ��� ����
		 * �����ٷ��� ���� ���� ��
		 */
		class IJob
		{
		public:
			virtual void Execute() = 0;
		};

		/*
		 * ��ϵ� IJob�� Node�� ž��Ǿ� ���� ��
		 * IJob�� �״�� ������ �ڵ鸵 �ϸ� �ȵ�
		 * ��ϵ� Job�� ����� ������ �� ����
		 */
		class JobNode
		{
		public:
			JobNode(IJob* job, int freq, int count);
			~JobNode();

			void Execute();
			bool IsExfired();

		private:
			IJob* mJob;
			int mFreq;
			int mCount;

			DWORD mNextTick;

		};

		/*
		 * Job���� �����ϴ� ��Ŀ
		 * ���� ������
		 */
		class Worker
		{
		public:
			Worker();
			~Worker();

			void AppendJob(IJob* job, int freq, int count);
			void Run();
			void Release();

		private:
			std::mutex mLock;
			std::list<JobNode*> mJobNodes;
			std::thread mThread;
			bool mIsExit;
		};

		/*
		 * Job�� ����ϰ� ����ϱ����� �Ŵ���
		 * ����� �������̽�
		 */
		class JobManager
		{
		public:
			JobManager();
			~JobManager();

			void Register(IJob* job, int freq, int count);
			void Release();

			static JobManager& GetInstance();

		private:
			std::mutex mLock;
			int mWorkerCount;
			std::vector<Worker*> mWorkers;
		};
	}
}