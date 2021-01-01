#include "CrownPCH.h"
#include "PathMovement.h"

#include "Core/ECS/Component/TransformComponentSystem.h"

namespace Crown
{
	PathMovement::PathMovement()
	{
		ASSERT_ENGINE(Application::Get().GetComponentSystemManager()->GetSystem<TransformComponentSystem>() != nullptr, "Transform ECSystem does not exists yet, create before creating the PathMovement ComponentSystemManager.");

		m_Transform = Application::Get().GetComponentSystemManager()->GetSystem<TransformComponentSystem>();
	}

	void PathMovement::OnUpdate(float a_DeltaTime)
	{
#ifdef CROWN_PC
		for (auto path : GetEntities())
		{
			//checks if m_path is empty and if so it updates the path
			if(m_Data.at(path).m_Path.empty())
			{
				UpdatePath(path);
			}
			
			//update the path for the players position
			if (!(m_Data.at(path).m_Player == NULL))
			{
				// see if close enough to player to start following the player
				if (glm::distance(m_Transform->GetLocalPosition(path), m_Transform->GetLocalPosition(m_Data.at(path).m_Player)) <= m_AcceptanceRange)
				{
					SetFollowingPlayer(path, true);
				}
				else
				{
					SetFollowingPlayer(path, false);
				}

				//if player moved out of specific range then update the path
				if (glm::distance(m_Data.at(path).m_OldPlayerPosition, m_Transform->GetLocalPosition(m_Data.at(path).m_Player)) >= m_AcceptanceRange)
				{
					UpdatePath(path);
				}
			}

			//Movement or update the direction for the flocking movement
			if (!IsFollowingPlayer(path))
			{
				if (!IsFlockingMovement(path))
				{
					if (m_Data.at(path).m_Moving)
					{
						glm::vec3 tempVec3 = m_Data.at(path).m_CurrentDirection * m_Data.at(path).m_MovementSpeed * a_DeltaTime;
						tempVec3 += m_Transform->GetLocalPosition(path);
						m_Transform->SetLocalPosition(path, tempVec3);
						if (glm::distance(m_Transform->GetLocalPosition(path), m_Data.at(path).m_Path.at(m_Data.at(path).m_CurrentPathIndex)->worldPosition) <= m_AcceptanceRange)
						{
							printf("\r\n Next Waypoint");
							m_Data.at(path).m_CurrentPathIndex = m_Data.at(path).m_CurrentPathIndex - 1;
							if (m_Data.at(path).m_CurrentPathIndex < 0)
							{
								m_Data.at(path).m_Moving = false;
								printf("\r\n End Reached");
								m_Data.at(path).m_Path.clear();
								return;
							}
							UpdateDirection(path);
						}
					}
				}
				else
				{
					if (glm::distance(m_Transform->GetLocalPosition(path), m_Data.at(path).m_Path.at(m_Data.at(path).m_CurrentPathIndex)->worldPosition) <= m_AcceptanceRange)
					{
						printf("\r\n Next Waypoint");
						m_Data.at(path).m_CurrentPathIndex = m_Data.at(path).m_CurrentPathIndex - 1;
						if (m_Data.at(path).m_CurrentPathIndex < 0)
						{
							m_Data.at(path).m_Moving = false;
							printf("\r\n End Reached");
							m_Data.at(path).m_Path.clear();
							return;
						}
						UpdateDirection(path);
					}
				}
			}

			if (IsFollowingPlayer(path))
			{
				//need to find a more efficient way for this
				UpdateDirection(path);
				
				glm::vec3 tempVec3 = m_Data.at(path).m_CurrentDirection * m_Data.at(path).m_MovementSpeed * a_DeltaTime;
				tempVec3 += m_Transform->GetLocalPosition(path);
				m_Transform->SetLocalPosition(path, tempVec3);
			}

		}
#endif
	}

	void PathMovement::SetPath(Entity a_Entity, std::vector<Path_Node*>& a_PathVec)
	{
		m_Data.at(a_Entity).m_Path = a_PathVec;
		m_Data.at(a_Entity).m_CurrentPathIndex = static_cast<int>(a_PathVec.size()) - 2;
		UpdateDirection(a_Entity);
	}

	void PathMovement::UpdatePath(Entity a_Entity)
	{
#ifdef CROWN_PC
		m_Data.at(a_Entity).m_OldPlayerPosition = m_Transform->GetLocalPosition(m_Data.at(a_Entity).m_Player);
		auto path = m_Data.at(a_Entity).m_PathSystem->FindPath(m_Transform->GetLocalPosition(a_Entity), m_Transform->GetLocalPosition(m_Data.at(a_Entity).m_Player));
		SetPath(a_Entity, path);
#endif
	}

	void PathMovement::UpdatePath(const std::vector<Entity>& a_Entities)
	{
		for (const auto& entity : a_Entities)
		{
			UpdatePath(entity);
		}
	}

	void PathMovement::SetPathSystem(Entity a_Entity, Path_System* a_PathSystem)
	{
		m_Data.at(a_Entity).m_PathSystem = a_PathSystem;
	}

	void PathMovement::SetPathSystem(const std::vector<Entity>& a_Entities, Path_System* a_PathSystem)
	{
		for (const auto& entity : a_Entities)
		{
			SetPathSystem(entity, a_PathSystem);
		}
	}

	glm::vec3 PathMovement::UpdateDirection(Entity a_Entity)
	{
		if (!IsFollowingPlayer(a_Entity))
		{
			//printf("%i", m_Data.at(a_Entity).m_CurrentPathIndex);
			glm::vec3 directionVec = m_Data.at(a_Entity).m_Path.at(m_Data.at(a_Entity).m_CurrentPathIndex)->worldPosition - m_Transform->GetLocalPosition(a_Entity);
			directionVec = normalize(directionVec);
			m_Data.at(a_Entity).m_CurrentDirection = directionVec;
			return directionVec;
		}
		
		glm::vec3 directionVec = m_Transform->GetLocalPosition(m_Data.at(a_Entity).m_Player) - m_Transform->GetLocalPosition(a_Entity);
		directionVec = normalize(directionVec);
		m_Data.at(a_Entity).m_CurrentDirection = directionVec;
		return directionVec;

	}

	glm::vec3 PathMovement::GetDirection(Entity a_Entity)
	{
		return m_Data.at(a_Entity).m_CurrentDirection;
	}

	void PathMovement::SetMoving(Entity a_Entity, bool a_Boolean)
	{
		m_Data.at(a_Entity).m_Moving = a_Boolean;
	}

	void PathMovement::SetMovementSpeed(Entity a_Entity, const float& a_MovementSpeed)
	{
		m_Data.at(a_Entity).m_MovementSpeed = a_MovementSpeed;
	}

	bool PathMovement::IsMoving(Entity a_Entity)
	{
		return m_Data.at(a_Entity).m_Moving;
	}

	void PathMovement::SetFlockingMovement(Entity a_Entity, bool a_Boolean)
	{
		m_Data.at(a_Entity).m_FlockingMovement = a_Boolean;
	}

	void PathMovement::SetFlockingMovement(const std::vector<Entity>& a_Entities, bool a_Boolean)
	{
		for (const auto& entity : a_Entities)
		{
			SetFlockingMovement(entity, a_Boolean);
		}
	}

	bool PathMovement::IsFlockingMovement(Entity a_Entity)
	{
		return m_Data.at(a_Entity).m_FlockingMovement;
	}

	void PathMovement::SetFollowingPlayer(Entity a_Entity, bool a_Boolean)
	{
		m_Data.at(a_Entity).m_FollowingPlayer = a_Boolean;
	}

	bool PathMovement::IsFollowingPlayer(Entity a_Entity)
	{
		return m_Data.at(a_Entity).m_FollowingPlayer;
	}

	void PathMovement::SetPlayer(Entity a_Entity, Entity a_Player)
	{
		m_Data.at(a_Entity).m_Player = a_Player;
	}

	Entity PathMovement::GetPlayer(Entity a_Entity)
	{
		return m_Data.at(a_Entity).m_Player;
	}

	void PathMovement::OnAddEntity(Entity a_Entity)
	{
		ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");

		std::pair<Entity, Data> pair{ a_Entity, {} };
		m_Data.insert(pair);
	}

	void PathMovement::OnRemoveEntity(Entity a_Entity)
	{
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

		m_Data.erase(a_Entity);
	}
}