#pragma once
#include "Core/Application.h"

#include <memory>
#include <vector>
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
	class RenderComponentSystem;
	class TransformComponentSystem;
}

class CrowdKing : public Crown::Application
{
protected:
    void OnUpdate(float a_DeltaTime) override;
    void OnRender() override;
    void OnStart() override;
    void OnLateUpdate(float a_DeltaTime) override;
    void OnFixedUpdate() override;
    void OnImGuiRender() override;

private:
	
};
