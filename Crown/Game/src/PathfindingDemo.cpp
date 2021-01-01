#include "PathfindingDemo.h"
#include "Core/Utilities/Log.h"

#include "Mode.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"
#include "Core/Pathfinding/PathMovement.h"
#include "Core/Renderer/Models.h"


#ifdef PATHFINDINGDEMO
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
    return new PathfindingDemo();
}
#endif

void PathfindingDemo::OnUpdate(float a_DeltaTime)
{
}

void PathfindingDemo::OnRender()
{

}

void PathfindingDemo::OnStart()
{
#ifdef CROWN_PC
    const int gridsize = 500;
    const float noderadius = 16.f;
    glm::ivec2 tempvec = glm::ivec2(gridsize, gridsize);
    m_PathGrid = Path_Grid(tempvec, noderadius, true);
    m_PathGrid.CreateGrid();

    auto models = GetResourceManager()->Get<Crown::Models>("../Engine/assets/models/BarramundiFish.gltf");

    for (int x = 0; x < m_PathGrid.gridSizeX; x++)
    {
        for (int y = 0; y < m_PathGrid.gridSizeY; y++)
        {
            if (!m_PathGrid.grid[x][y].Walkable)
            {
            	auto tempNode = m_PathGrid.grid[x][y];
                Crown::Entity entity = Crown::Entity::Create();
                m_Transform = entity.AddToSystem<Crown::TransformComponentSystem>();
                m_Transform->SetLocalPosition(entity, glm::vec3(tempNode.worldPosition.x, tempNode.worldPosition.y, -15.0f));
                m_Transform->SetLocalScale(entity, glm::vec3(0.5, 0.5, 0.5));
                m_Drawable = entity.AddToSystem<Crown::RenderComponentSystem>();
                m_Drawable->SetModel(entity, models->GetModels().at(0));
                m_Entities.push_back(entity);
            }
        }
    }
    m_PathSystem.SetPathGrid(m_PathGrid);
    //auto path = m_PathSystem.FindPath(&m_PathGrid.grid[1][m_PathGrid.gridSizeY - 4], &m_PathGrid.grid[m_PathGrid.gridSizeX - 4][2]);

    Crown::Entity player = Crown::Entity::Create();
    {
        auto tempNode = m_PathGrid.grid[m_PathGrid.gridSizeX - 4][2];
        
        m_Transform = player.AddToSystem<Crown::TransformComponentSystem>();
        m_Transform->SetLocalPosition(player, glm::vec3(tempNode.worldPosition.x, tempNode.worldPosition.y, -15.0f));
        m_Transform->SetLocalScale(player, glm::vec3(0.4, 0.4, 0.4));
        m_Drawable = player.AddToSystem<Crown::RenderComponentSystem>();
        m_Drawable->SetModel(player, models->GetModels().at(0));
        m_Entities.push_back(player);
    }

    {
        auto tempNode = m_PathGrid.grid[1][m_PathGrid.gridSizeY - 4];
        Crown::Entity entity = Crown::Entity::Create();
        m_Transform = entity.AddToSystem<Crown::TransformComponentSystem>();
        m_Transform->SetLocalPosition(entity, glm::vec3(tempNode.worldPosition.x, tempNode.worldPosition.y, -15.0f));
        m_Transform->SetLocalScale(entity, glm::vec3(0.4, 0.4, 0.4));
        m_Drawable = entity.AddToSystem<Crown::RenderComponentSystem>();
        m_Drawable->SetModel(entity, models->GetModels().at(0));
        m_PathMovement = entity.AddToSystem<Crown::PathMovement>();
        m_PathMovement->SetPathSystem(entity, &m_PathSystem);
        m_PathMovement->SetMovementSpeed(entity, 1.0f);
        m_PathMovement->SetPlayer(entity, player);
        m_PathMovement->SetMoving(entity, true);
        m_Entities.push_back(entity);
        m_Entity = entity;
    }
#endif
}

void PathfindingDemo::OnLateUpdate(float a_DeltaTime)
{
}

void PathfindingDemo::OnFixedUpdate()
{
}

void PathfindingDemo::OnImGuiRender()
{
}