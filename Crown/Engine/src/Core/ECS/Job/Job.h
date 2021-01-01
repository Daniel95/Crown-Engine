#pragma once
#include "Core/ECS/Component/ComponentHelper.h"

#include "Core/ECS/Job/JobManager.h"

namespace Crown
{
	class Job
	{
	public:
		Job() = default;
		virtual ~Job() = default;
		
		void Update();
		virtual void OnUpdate(int a_ThreadId) = 0;
		
		template <typename T1, typename T2>
		void Schedule(const std::function<void(const std::vector<int>&, std::unordered_map<int, T1>&, std::unordered_map<int, T2>&)>& a_Func, int a_ThreadId = -1);

		template <typename T1, typename T2>
		void Run(const std::function<void(const std::vector<int>&, std::unordered_map<int, T1>&, std::unordered_map<int, T2>&)>& a_Func);

	};

	inline void Job::Update()
	{
		OnUpdate(-1);
	}

	template <typename T1, typename T2>
	void Job::Schedule(const std::function<void(const std::vector<int>&, std::unordered_map<int, T1>&, std::unordered_map<int, T2>&)>& a_Func, int a_ThreadId)
	{
		const std::function<void(std::function<void()>)> task = [a_Func](const std::function<void()> a_CallBack)
		{
			std::vector<int> entitiesWithComponents = ComponentHelper::GetEntitiesWith<T1, T2>();

			std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
			std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();

			a_Func(entitiesWithComponents, components1, components2);

			if(a_CallBack != nullptr)
			{
				a_CallBack();
			}
		};

		Application::Get().GetJobManager()->ScheduleTask(task, a_ThreadId);
	}

	template <typename T1, typename T2>
	void Job::Run(
		const std::function<void(const std::vector<int>&, std::unordered_map<int, T1>&, std::unordered_map<int, T2>&)>& a_Func)
	{
		std::vector<int> entitiesWithComponents = ComponentHelper::GetEntitiesWith<T1, T2>();

		std::unordered_map<int, T1>& components1 = ComponentManager<T1>::GetComponents();
		std::unordered_map<int, T2>& components2 = ComponentManager<T2>::GetComponents();

		a_Func(entitiesWithComponents, components1, components2);
	}
}
