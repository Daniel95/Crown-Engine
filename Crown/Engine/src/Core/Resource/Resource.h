#pragma once

#include "Core/Utilities/Utilities.h"

#include <string>

namespace Crown
{
    class Resource
    {
    public:
        virtual ~Resource() = default;
        virtual void Reload() {}
        const std::string& GetPath() const { return m_Path; }
        const std::string& GetName() const { return m_Name; }
        void InitPath(const std::string& a_Path);
        void UpdatePath(const std::string& a_Path);
        static ullong CalculateResourceId(const std::string& a_Path) { return StringHash(a_Path); }

    protected:
        std::string m_Name = "";
        std::string m_Path = "";

    private:
        static std::string ExtractName(const std::string& a_Path);

    };
}