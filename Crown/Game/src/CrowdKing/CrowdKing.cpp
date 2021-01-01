#include "CrowdKing.h"
#include <iostream>

#include <Core/Utilities/Log.h>
#include "Mode.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"

#ifdef CROWDKING
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
    return new CrowdKing();
}
#endif

void CrowdKing::OnUpdate(float a_DeltaTime)
{
	
    //LOG("Update CrowdKing");
}

void CrowdKing::OnRender()
{
    //LOG("Render CrowdKing");
}

void CrowdKing::OnStart()
{

}

void CrowdKing::OnLateUpdate(float a_DeltaTime)
{
}

void CrowdKing::OnFixedUpdate()
{
}

void CrowdKing::OnImGuiRender()
{
}
