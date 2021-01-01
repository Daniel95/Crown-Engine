#pragma once

#include <vector>
#include <memory>

namespace Crown
{
	class ComponentSystem;

	class ComponentSystemManager
	{
	public:
		ComponentSystemManager() = default;
		~ComponentSystemManager() = default;
		
		template <typename  T>
		std::shared_ptr<T> AddSystem();
		template <typename  T>
		void RemoveSystem();
		template <typename  T>
		std::shared_ptr<T> GetSystem() const;

		const std::vector<std::shared_ptr<ComponentSystem>>& GetAllSystems() const { return m_Systems; }

		void Update(float a_DeltaTime) const;

	private:
		std::vector<std::shared_ptr<ComponentSystem>> m_Systems;
		
	};

	template <typename T>
	std::shared_ptr<T> ComponentSystemManager::AddSystem()
	{
		static_assert(std::is_base_of<ComponentSystem, T>::value, "Type must derive from ComponentSystem");

		std::shared_ptr<T> type = std::make_shared<T>();

		const std::shared_ptr<ComponentSystem> ecsSystem = std::dynamic_pointer_cast<ComponentSystem>(type);
		m_Systems.push_back(ecsSystem);

		return type;
	}

	template <typename T>
	void ComponentSystemManager::RemoveSystem()
	{
		static_assert(std::is_base_of<ComponentSystem, T>::value, "Type must derive from ComponentSystem");

		std::shared_ptr<T> ecsSystemOfType = GetSystem<T>();

		ASSERT_ENGINE(ecsSystemOfType != nullptr, "Can't remove system because this system doesn't exists.");

		const std::shared_ptr<ComponentSystem> component = std::dynamic_pointer_cast<ComponentSystem>(ecsSystemOfType);

		m_Systems.erase(std::remove(m_Systems.begin(), m_Systems.end(), component), m_Systems.end());
	}

	template <typename T>
	std::shared_ptr<T> ComponentSystemManager::GetSystem() const
	{
		CROWN_PROFILE_FUNCTION();
		static_assert(std::is_base_of<ComponentSystem, T>::value, "Type must derive from ComponentSystem");

		std::shared_ptr<T> ecsSystemOfType = nullptr;

		for (const auto& component : m_Systems)
		{
			ecsSystemOfType = std::dynamic_pointer_cast<T>(component);

			if (ecsSystemOfType != nullptr)
			{
				break;
			}
		}

		return ecsSystemOfType;
	}
}
