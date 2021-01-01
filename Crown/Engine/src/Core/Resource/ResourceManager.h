#pragma once

#include "Core/Utilities/Defines.h"
#include "Core/Resource/Resource.h"

#include <unordered_map>
#include <memory>

namespace Crown
{
    class SystemManager;

    class ResourceManager
    {
    public:
        void Add(const std::string& a_Name, const std::shared_ptr<Resource>& a_Resource);
        template<typename T, typename... Args>
        std::shared_ptr<T> Load(const std::string& a_Path);
        template<typename T>
        std::shared_ptr<T> Get(const std::string& a_Path);
        template<typename T>
        std::shared_ptr<T> GetLoaded(const std::string& a_Path);
        template<typename T>
        std::vector<std::shared_ptr<T>> GetResourcesOfType();
        bool HasResource(const std::string& a_Path);
        template<typename T>
        void UnloadResource(const std::string& a_Path);
        std::vector<std::shared_ptr<Resource>> GetAllResources();
        void UpdateResourcePath(const std::string& a_OldPath, const std::string& a_NewPath);

    protected:

        /// A map of resources
        std::unordered_map<ullong, std::shared_ptr<Resource>> m_Resources;
    };

    template<typename T>
    std::vector<std::shared_ptr<T>> ResourceManager::GetResourcesOfType()
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        std::vector<std::shared_ptr<T>> resources;

        for (auto const& x : m_Resources)
        {
            T* resourceOfType = dynamic_cast<T*>(x.second);

            if (resourceOfType != nullptr)
            {
                resources.push_back(resourceOfType);
            }
        }

        return resources;
    }

    template<typename T, typename... Args>
    std::shared_ptr<T> ResourceManager::Load(const std::string& a_Path)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");
        ASSERT_ENGINE(!HasResource(a_Path), "Resource is already loaded!");

        ullong id = Resource::CalculateResourceId(a_Path);

        std::shared_ptr<T> resource = T::Load(a_Path);

        std::dynamic_pointer_cast<Resource>(resource)->InitPath(a_Path);

        m_Resources.insert(std::make_pair(id, resource));

        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::Get(const std::string& a_Path)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

        std::shared_ptr<T> resource = GetLoaded<T>(a_Path);

        if(resource == nullptr)
        {
            resource = Load<T>(a_Path);
        }
    	
        ASSERT_ENGINE(resource != nullptr, "Resource was not loaded correctly!");
    	
        return resource;
    }

    template <typename T>
    std::shared_ptr<T> ResourceManager::GetLoaded(const std::string& a_Path)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Resource");

		const ullong id = Resource::CalculateResourceId(a_Path);

		const auto it = m_Resources.find(id);

        // Check cache
        if (it != m_Resources.end())
        {
            return std::dynamic_pointer_cast<T>(it->second);
        }

        return nullptr;
    }

    template<typename T>
    void ResourceManager::UnloadResource(const std::string& a_Path)
    {
        static_assert(std::is_base_of<Resource, T>::value, "Type must derive from Component");

        ullong id = T::CalculateResourceId(a_Path);

        ASSERT_ENGINE(m_Resources.find(id) != m_Resources.end(), "Resource does not exists!");
        ASSERT_ENGINE(m_Resources[id].use_count() >= 1, "Resource " + a_Path + " that you are trying to delete is still in use!");

        m_Resources.erase(id);
    }
}
