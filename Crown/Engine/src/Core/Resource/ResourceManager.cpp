#include "CrownPCH.h"

#include "Core/Resource/ResourceManager.h"

#include <string>

namespace Crown
{
    void ResourceManager::Add(const std::string& a_Name, const std::shared_ptr<Resource>& a_Resource)
    {
        CROWN_PROFILE_FUNCTION();
    	
        ullong id = Resource::CalculateResourceId(a_Name);

        ASSERT_ENGINE(a_Name.find('/') == std::string::npos, "Use Load(a_FilePath) instead of Create when trying to load a file.");
        ASSERT_ENGINE(m_Resources.find(id) == m_Resources.end(), "Resource with name " + a_Name + " already exists!");

        std::dynamic_pointer_cast<Resource>(a_Resource)->InitPath(a_Name);

        m_Resources.insert(std::make_pair(id, a_Resource));
    }

	bool ResourceManager::HasResource(const std::string& a_Path)
	{
        CROWN_PROFILE_FUNCTION();

		const ullong id = Resource::CalculateResourceId(a_Path);
        return m_Resources.find(id) != m_Resources.end();
	}

    std::vector<std::shared_ptr<Resource>> ResourceManager::GetAllResources()
    {
        CROWN_PROFILE_FUNCTION();

        std::vector<std::shared_ptr<Resource>> resources;

        for (std::pair<ullong, std::shared_ptr<Resource>> resource : m_Resources)
        {
            resources.push_back(resource.second);
        }

        return resources;
    }

    void ResourceManager::UpdateResourcePath(const std::string& a_OldPath, const std::string& a_NewPath)
    {
        CROWN_PROFILE_FUNCTION();

        const ullong oldId = Resource::CalculateResourceId(a_OldPath);
        const ullong newId = Resource::CalculateResourceId(a_NewPath);

        const std::shared_ptr<Resource> resource = m_Resources.at(oldId);

        m_Resources.at(newId) = resource;

        m_Resources.erase(oldId);
    }
}
