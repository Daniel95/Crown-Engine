#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <thread>

namespace Crown
{
	enum class JobType
	{
		Gameplay,
		Renderer,
	};
	
	class Job;
	
	class JobManager
	{
	public:
		JobManager();
		~JobManager() = default;

		template <typename  T>
		std::shared_ptr<T> Add(JobType a_JobType);
		//template <typename  T>
		//void Remove();
		template <typename  T>
		std::shared_ptr<T> Get() const;

		//const std::vector<std::shared_ptr<Job>>& GetAll() const { return m_Jobs; }

		void Update(JobType a_JobType);
		void ScheduleTask(std::function<void(std::function<void()>)> a_Task, int a_ThreadDependency = -1);
		void OnTaskComplete(unsigned int a_ThreadId);
		
	protected:
		unsigned int GetLeastPopulatedThread();
		void WaitForAllThreads();
		void ResetThreadTaskIndexes();

	private:
		std::unordered_map<JobType, std::vector<std::shared_ptr<Job>>> m_Jobs;
		std::vector<std::thread> m_Threads;
		std::unordered_map<unsigned int, std::vector<std::function<void(std::function<void()>)>>> m_TaskQueue;
		std::vector<int> m_ThreadsTaskIndex;
		
	};

	template <typename T>
	std::shared_ptr<T> JobManager::Add(JobType a_JobType)
	{
		static_assert(std::is_base_of<Job, T>::value, "Type must derive from Job");

		std::shared_ptr<T> type = std::make_shared<T>();

		if(m_Jobs.find(a_JobType) == m_Jobs.end())
		{
			std::pair<JobType, std::vector<std::shared_ptr<Job>>> pair = { a_JobType, std::vector<std::shared_ptr<Job>>() };
			m_Jobs.insert(pair);
		}
		
		const std::shared_ptr<Job> job = std::dynamic_pointer_cast<Job>(type);
		m_Jobs.at(a_JobType).push_back(job);

		return type;
	}

	/*
	template <typename T>
	void JobManager::Remove()
	{
		static_assert(std::is_base_of<Job, T>::value, "Type must derive from Job");

		std::shared_ptr<T> ecsSystemOfType = Get<T>();

		ASSERT_ENGINE(ecsSystemOfType != nullptr, "Can't remove system because this system doesn't exists.");

		const std::shared_ptr<Job> component = std::dynamic_pointer_cast<Job>(ecsSystemOfType);

		m_Jobs.erase(std::remove(m_Jobs.begin(), m_Jobs.end(), component), m_Jobs.end());
	}
	*/

	template <typename T>
	std::shared_ptr<T> JobManager::Get() const
	{
		static_assert(std::is_base_of<Job, T>::value, "Type must derive from Job");

		std::shared_ptr<T> jobOfType = nullptr;

		for (const auto& pair : m_Jobs)
		{
			for (const auto& job : pair.second)
			{
				jobOfType = std::dynamic_pointer_cast<T>(job);

				if (jobOfType != nullptr)
				{
					break;
				}
			}
		}

		return jobOfType;
	}
}
