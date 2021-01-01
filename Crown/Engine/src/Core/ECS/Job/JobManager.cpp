#include "CrownPCH.h"
#include "JobManager.h"

#include "Core/ECS/Job/Job.h"
#include <memory>

namespace Crown
{
	JobManager::JobManager()
	{
		unsigned int maxThreadCount = std::thread::hardware_concurrency();

		for (size_t i = 0; i < maxThreadCount; i++)
		{
			std::thread thread;
			m_Threads.push_back(std::move(thread));

			std::pair<unsigned int, std::vector<std::function<void(std::function<void()>)>>> taskQueuePair
			{
				static_cast<unsigned int>(i), std::vector<std::function<void(std::function<void()>)>>()
			};
			
			m_TaskQueue.insert(taskQueuePair);

			m_ThreadsTaskIndex.push_back(0);
		}
	}

	void JobManager::Update(JobType a_JobType)
	{
		CROWN_PROFILE_FUNCTION();

		for (const auto& pair : m_Jobs)
		{
			for (const auto& job : pair.second)
			{
				job->Update();
			}
		}

		for(auto& tasks : m_TaskQueue)
		{
			if(tasks.second.empty()) { continue; }
			
			unsigned int threadId = static_cast<unsigned int>(tasks.first);

			std::function<void(std::function<void()>)> firstTask = tasks.second.at(0);
			std::function<void()> callBack = [this, threadId]() { OnTaskComplete(threadId); };

			m_Threads[threadId] = std::thread(firstTask, callBack);
		}

		WaitForAllThreads();
		ResetThreadTaskIndexes();
	}

	void JobManager::ScheduleTask(std::function<void(std::function<void()>)> a_Task, int a_ThreadId)
	{
		unsigned int threadId = a_ThreadId != -1 ? a_ThreadId : GetLeastPopulatedThread();
		
		m_TaskQueue[threadId].push_back(a_Task);
	}

	void JobManager::OnTaskComplete(unsigned int threadId)
	{
		
	}

	//TODO: Implement;
	unsigned int JobManager::GetLeastPopulatedThread()
	{
		return 0;
	}

	void JobManager::WaitForAllThreads()
	{
		for (auto& thread : m_Threads)
		{
			if(thread.joinable())
			{
				thread.join();
			}
		}
	}

	void JobManager::ResetThreadTaskIndexes()
	{
		for (size_t i = 0; i < m_ThreadsTaskIndex.size(); i++)
		{
			m_ThreadsTaskIndex[i] = 0;
		}
	}
}
