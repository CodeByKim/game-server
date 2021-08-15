#include "./Util/JobManager/JobManager.h"

namespace garam
{
	namespace jobsystem
	{
		JobNode::JobNode(IJob* job, int freq, int count)
			: mJob(job)
			, mFreq(freq)
			, mCount(count)
			, mNextTick(timeGetTime() + mFreq)
		{
		}

		JobNode::~JobNode()
		{
		}

		void JobNode::Execute()
		{
			DWORD current = timeGetTime();

			if (current >= mNextTick)
			{
				mNextTick = timeGetTime() + mFreq;
				mJob->Execute();
				mCount -= 1;
			}
		}

		bool JobNode::IsExfired()
		{
			if (mCount == JOB_INFINITY)
			{
				return false;
			}

			if (mCount == 0)
			{
				return true;
			}

			return false;
		}

		Worker::Worker()
		{
			mIsExit = false;

			mThread = std::thread(&Worker::Run, this);
		}

		Worker::~Worker()
		{
			for (JobNode* node : mJobNodes)
			{
				delete node;
			}

			mJobNodes.clear();
		}

		void Worker::AppendJob(IJob* job, int freq, int count)
		{
			JobNode* node = new JobNode(job, freq, count);

			mLock.lock();
			mJobNodes.push_back(node);
			mLock.unlock();
		}

		void Worker::Run()
		{
			while (!mIsExit)
			{
				std::vector<JobNode*> deleteJobs;

				mLock.lock();
				for (JobNode* node : mJobNodes)
				{
					if (node->IsExfired())
					{
						deleteJobs.push_back(node);
						continue;
					}

					node->Execute();
				}
				mLock.unlock();

				for (JobNode* node : deleteJobs)
				{
					mJobNodes.remove(node);
					delete node;
				}

				Sleep(0);
			}
		}

		void Worker::Release()
		{
			mIsExit = true;
			mThread.join();
		}

		JobManager::JobManager()
		{
			mWorkerCount = 4;

			for (int i = 0; i < mWorkerCount; i++)
			{
				Worker* worker = new Worker();
				mWorkers.push_back(worker);
			}
		}

		JobManager::~JobManager()
		{
			for (int i = 0; i < mWorkerCount; i++)
			{
				delete mWorkers[i];
			}

			mWorkers.clear();
		}

		void JobManager::Register(IJob* job, int freq, int count)
		{
			static int nodeCount = 0;
			int index = nodeCount % mWorkerCount;

			mLock.lock();
			Worker* worker = mWorkers[index];
			mLock.unlock();

			worker->AppendJob(job, freq, count);
			nodeCount += 1;
		}

		void JobManager::Release()
		{
			for (int i = 0; i < mWorkerCount; i++)
			{
				mWorkers[i]->Release();
			}
		}

		JobManager& JobManager::GetInstance()
		{
			static JobManager instance;
			return instance;
		}
	}
}