#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#undef countof
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>
#undef countof

#include "Core/ECS/System/ComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include "TransformHelper.h"

namespace Crown
{
	struct TransformComponent
	{
		glm::vec3 localPosition = glm::vec3(0);
		glm::vec3 localScale = glm::vec3(1);
		glm::vec3 localRotation = glm::vec3(0);

		glm::mat4 worldModelMatrix = glm::identity<glm::mat4>();

		std::vector<Entity> children;
		Entity* parent = nullptr;
		bool dirty = true;
	};
	
	class TransformComponentSystem : public ComponentSystem
	{
	public:
		TransformComponentSystem() = default;
		~TransformComponentSystem() = default;

		virtual void OnUpdate(float a_DeltaTime) override { };
		virtual void OnAddEntity(Entity a_Entity) override;
		virtual void OnRemoveEntity(Entity a_Entity) override;

		void SetLocalPosition(Entity a_Entity, const glm::vec3& a_Position);
		void SetLocalPositions(const std::vector<Entity>& a_Entities, const glm::vec3& a_Position);
		const glm::vec3& GetLocalPosition(Entity a_Entity) const { return m_Data.at(a_Entity).localPosition; }
		void GetLocalPositions(std::vector<glm::vec3>& a_Positions, const std::vector<Entity>& a_Entities) const;
		void TranslateLocal(Entity a_Entity, const glm::vec3& a_Movement) { m_Data.at(a_Entity).localPosition += a_Movement; }
		void TranslateLocal(const std::vector<Entity>& a_Entities, const glm::vec3& a_Movement);

		void SetLocalScale(Entity a_Entity, const glm::vec3& a_Scale);
		void SetLocalScales(const std::vector<Entity>& a_Entities, const glm::vec3& a_Scale);
		const glm::vec3& GetLocalScale(Entity a_Entity) const { return m_Data.at(a_Entity.GetId()).localScale; }
		void GetLocalScales(std::vector<glm::vec3>& a_Scales, const std::vector<Entity>& a_Entities) const;
		void ScaleLocal(Entity a_Entity, const glm::vec3& a_Scaling) { m_Data.at(a_Entity).localScale += a_Scaling; }
		void ScaleLocal(const std::vector<Entity>& a_Entities, const glm::vec3& a_Scaling);

		void SetLocalRotation(Entity a_Entity, const glm::vec3& a_Rotation);
		void SetLocalRotations(const std::vector<Entity>& a_Entities, const glm::vec3& a_Rotation);
		const glm::vec3& GetLocalRotation(Entity a_Entity) const { return m_Data.at(a_Entity).localRotation; }
		void GetLocalRotations(std::vector<glm::vec3>& a_Rotations, const std::vector<Entity>& a_Entities) const;

		void SetLookAt(Entity a_Entity, glm::vec3 a_Target);

		glm::vec3 InverseTransformPoint(Entity a_Entity, const glm::vec3& a_WorldPoint) { return TransformHelper::InverseTransformPoint(GetWorldModelMatrix(a_Entity), a_WorldPoint); } //World to local point 
		glm::vec3 InverseTransformVector(Entity a_Entity, const glm::vec3& a_WorldVector) { return TransformHelper::InverseTransformVector(GetWorldModelMatrix(a_Entity), a_WorldVector); } //World to local vector
		glm::vec3 TransformPoint(Entity a_Entity, const glm::vec3& a_LocalPoint) { return TransformHelper::TransformPoint(GetWorldModelMatrix(a_Entity), a_LocalPoint); } //Local to world point
		glm::vec3 TransformVector(Entity a_Entity, const glm::vec3& a_LocalVector) { return TransformHelper::TransformVector(GetWorldModelMatrix(a_Entity), a_LocalVector); } //Local to world vector
		
		glm::vec3 GetRight(Entity a_Entity) { return TransformVector(a_Entity, glm::vec3(1, 0, 0)); }
		glm::vec3 GetUp(Entity a_Entity) { return TransformVector(a_Entity, glm::vec3(0, 1, 0)); }
		glm::vec3 GetForward(Entity a_Entity) { return TransformVector(a_Entity, glm::vec3(0, 0, 1)); }

		glm::vec3 GetWorldPosition(Entity a_Entity);
		glm::vec3 GetWorldScale(Entity a_Entity);
		glm::mat4 CalculateWorldRotationMatrix(Entity a_Entity);
		
		const glm::mat4& GetWorldModelMatrix(Entity a_Entity);
		void GetWorldModelMatrix(std::vector<glm::mat4>& a_Models, const std::vector<Entity>& a_Entities);
		
	private:

		std::unordered_map<int, TransformComponent> m_Data;
	};
}
