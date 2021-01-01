#pragma once
#include "Core/ECS/System/ComponentSystem.h"
#include <unordered_map>
#include <vector>
#include "Path_Node.h"
#include "Core/ECS/Entity/Entity.h"
#include "Path_System.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>

namespace Crown
{
	class TransformComponentSystem;

	class PathMovement : public ComponentSystem
	{
	public:
		PathMovement();
		~PathMovement() = default;

		virtual void OnUpdate(float a_DeltaTime) override;
		virtual void OnAddEntity(Entity a_Entity) override;
		virtual void OnRemoveEntity(Entity a_Entity) override;

		void SetPath(Entity a_Entity, std::vector<Path_Node*>& a_PathVec);

		void UpdatePath(Entity a_Entity);
		void UpdatePath(const std::vector<Entity>& a_Entities);
		
		void SetPathSystem(Entity a_Entity, Path_System* a_PathSystem);
		void SetPathSystem(const std::vector<Entity>& a_Entities, Path_System* a_PathSystem);

		glm::vec3 UpdateDirection(Entity a_Entity);
		glm::vec3 GetDirection(Entity a_Entity);
		void SetMoving(Entity a_Entity, bool a_Boolean);
		void SetMovementSpeed(Entity a_Entity, const float& a_MovementSpeed);
		bool IsMoving(Entity a_Entity);

		void SetFlockingMovement(Entity a_Entity, bool a_Boolean);
		void SetFlockingMovement(const std::vector<Entity>& a_Entities, bool a_Boolean);
		bool IsFlockingMovement(Entity a_Entity);

		void SetFollowingPlayer(Entity a_Entity, bool a_Boolean);
		bool IsFollowingPlayer(Entity a_Entity);

		void SetPlayer(Entity a_Entity, Entity a_Player);
		Entity GetPlayer(Entity a_Entity);
		
	private:
		struct Data
		{
			std::vector<Path_Node*> m_Path;
			Path_System* m_PathSystem;
			glm::vec3 m_CurrentDirection;
			glm::vec3 m_OldPlayerPosition;
			Entity m_Player = NULL;			
			bool m_Moving = false;
			bool m_FlockingMovement = false;
			bool m_FollowingPlayer = false;
			float m_MovementSpeed = 30.f;
			int m_CurrentPathIndex = 0;
		};
		std::unordered_map<int, Data> m_Data;
		std::shared_ptr<TransformComponentSystem> m_Transform;
		const float m_AcceptanceRange = 2.0;
	};
}
