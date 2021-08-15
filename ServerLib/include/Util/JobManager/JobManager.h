#pragma once
#include "./Common/CommonLib.h"

#define JOB_INFINITY -1

namespace garam
{
	namespace jobsystem
	{
		/*
		 * IJob을 상속받고 가상함수를 구현
		 * 스케줄러에 의해 실행 됨
		 */
		class IJob
		{
		public:
			virtual void Execute() = 0;
		};

		/*
		 * 등록된 IJob은 Node에 탑재되어 실행 됨
		 * IJob을 그대로 가지고 핸들링 하면 안됨
		 * 등록된 Job은 맘대로 삭제할 수 없음
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
		 * Job들을 실행하는 워커
		 * 다중 스레드
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
		 * Job을 등록하고 사용하기위한 매니저
		 * 사용자 인터페이스
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