#include "CrownPCH.h"

#include "ExampleResource.h"

std::shared_ptr<ExampleResource> ExampleResource::Create()
{
    return std::make_shared<ExampleResource>();
}

std::shared_ptr<ExampleResource> ExampleResource::Load(const std::string& a_Path)
{
    return std::make_shared<ExampleResource>();
}
