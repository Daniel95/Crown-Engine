#pragma once
#include "Core/Application.h"
#include "Core/Pathfinding/Path_Grid.h"
#include "Core/Pathfinding/Path_System.h"

#include <memory>
#include <vector>
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
    class RenderComponentSystem;
    class TransformComponentSystem;
    class PathMovement;
}

class PathfindingDemo : public Crown::Application
{
private:
    Path_Grid m_PathGrid = Path_Grid();
    Path_System m_PathSystem = Path_System();
    //temp debug keys
	
    std::shared_ptr<Crown::TransformComponentSystem> m_Transform;
    std::shared_ptr<Crown::RenderComponentSystem> m_Drawable;
    std::shared_ptr<Crown::PathMovement> m_PathMovement;
	
    std::vector<Crown::Entity> m_Entities;
    Crown::Entity m_Entity = 0;
protected:
    void OnUpdate(float a_DeltaTime) override;
    void OnRender() override;
    void OnStart() override;
    void OnLateUpdate(float a_DeltaTime) override;
    void OnFixedUpdate() override;
    void OnImGuiRender() override;
};
