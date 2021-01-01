#pragma once
#include "Core/Resource/Resource.h"

#include <string>
#include <memory>

class ExampleResource : public Crown::Resource
{
public:
    static std::shared_ptr<ExampleResource> Create();
    static std::shared_ptr<ExampleResource> Load(const std::string& a_Path);

};
