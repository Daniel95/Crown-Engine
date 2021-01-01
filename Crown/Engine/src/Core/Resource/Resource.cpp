#include "CrownPCH.h"

#include "Resource.h"
#include "ResourceManager.h"
#include "Core/Application.h"

namespace Crown
{
    //Init's the path used for Resource ID, also updates the name.
    void Resource::InitPath(const std::string& a_Path)
    {
        CROWN_PROFILE_FUNCTION();
        ASSERT_ENGINE(m_Path.empty(), "Path already initialized, use UpdatePath!");

        m_Path = a_Path;
        m_Name = ExtractName(a_Path);
    }

    //Updates the path and change the resource ID accordingly, also updates the name.
    void Resource::UpdatePath(const std::string& a_Path)
    {
        CROWN_PROFILE_FUNCTION();
        ASSERT_ENGINE(!m_Path.empty(), "Path is unitialized, use InitPath!");
    	
        if (a_Path == m_Path) { return; }

        Application::Get().GetResourceManager()->UpdateResourcePath(m_Path, a_Path);

        m_Path = a_Path;
        m_Name = ExtractName(a_Path);
    }

    std::string Resource::ExtractName(const std::string& a_Path)
    {
        CROWN_PROFILE_FUNCTION();

        // Extract name from filepath
        // example: assets/shaders/FlatColor.glsl (between the last / and last .)
        auto lastSlash = a_Path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        const auto lastDot = a_Path.rfind('.');
        const auto count = lastDot == std::string::npos ? a_Path.size() - lastSlash : lastDot - lastSlash;

        return a_Path.substr(lastSlash, count);
    }
}
