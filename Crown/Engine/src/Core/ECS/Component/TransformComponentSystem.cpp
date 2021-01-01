#include "CrownPCH.h"
#include "TransformComponentSystem.h"

#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
	void TransformComponentSystem::OnAddEntity(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) == 0, "Entity already exists!");
		
		std::pair<int, TransformComponent> pair{ a_Entity, {} };
		m_Data.insert(pair);
	}

	void TransformComponentSystem::OnRemoveEntity(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

		m_Data.erase(a_Entity);
	}

	void TransformComponentSystem::SetLocalPosition(Entity a_Entity, const glm::vec3& a_Position)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
		
		auto it = m_Data.find(a_Entity);
		it->second.dirty = true;
		it->second.localPosition = a_Position;
	}

	void TransformComponentSystem::SetLocalPositions(const std::vector<Entity>& a_Entities,
									const glm::vec3& a_Position)
	{
		CROWN_PROFILE_FUNCTION();

		for(const auto& entity : a_Entities)
		{
			SetLocalPosition(entity, a_Position);
		}
	}

	void TransformComponentSystem::GetLocalPositions(std::vector<glm::vec3>& a_Positions, const std::vector<Entity>& a_Entities) const
	{
		CROWN_PROFILE_FUNCTION();

		if (a_Positions.size() != a_Entities.size())
		{
			a_Positions.resize(a_Entities.size());
		}

		for (size_t i = 0; i < a_Entities.size(); i++)
		{
			a_Positions[i] = GetLocalPosition(a_Entities[i]);
		}
	}

	void TransformComponentSystem::TranslateLocal(const std::vector<Entity>& a_Entities, const glm::vec3& a_Movement)
	{
		CROWN_PROFILE_FUNCTION();

		for (const auto& entity : a_Entities)
		{
			auto it = m_Data.find(entity);
			it->second.dirty = true;
			it->second.localPosition += a_Movement;
		}
	}

	void TransformComponentSystem::SetLocalScale(Entity a_Entity, const glm::vec3& a_Scale)
	{
		CROWN_PROFILE_FUNCTION();

		auto it = m_Data.find(a_Entity);
		it->second.dirty = true;
		it->second.localScale = a_Scale;
	}

	void TransformComponentSystem::SetLocalScales(const std::vector<Entity>& a_Entities, const glm::vec3& a_Scale)
	{
		CROWN_PROFILE_FUNCTION();

		for (const auto& entity : a_Entities)
		{
			SetLocalScale(entity, a_Scale);
		}
	}

	void TransformComponentSystem::GetLocalScales(std::vector<glm::vec3>& a_Scales, const std::vector<Entity>& a_Entities) const
	{
		CROWN_PROFILE_FUNCTION();

		if (a_Scales.size() != a_Entities.size())
		{
			a_Scales.resize(a_Entities.size());
		}

		for (size_t i = 0; i < a_Entities.size(); i++)
		{
			a_Scales[i] = GetLocalScale(a_Entities[i]);
		}
	}

	void TransformComponentSystem::ScaleLocal(const std::vector<Entity>& a_Entities, const glm::vec3& a_Scaling)
	{
		CROWN_PROFILE_FUNCTION();

		for (const auto& entity : a_Entities)
		{
			auto it = m_Data.find(entity);
			it->second.dirty = true;
			it->second.localScale += a_Scaling;
		}
	}

	void TransformComponentSystem::SetLocalRotation(Entity a_Entity, const glm::vec3& a_Rotation)
	{
		CROWN_PROFILE_FUNCTION();

		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
		
		auto it = m_Data.find(a_Entity);
		it->second.dirty = true;
		it->second.localRotation = a_Rotation;
	}

	void TransformComponentSystem::SetLocalRotations(const std::vector<Entity>& a_Entities, const glm::vec3& a_Rotation)
	{
		CROWN_PROFILE_FUNCTION();

		for (const auto& entity : a_Entities)
		{
			SetLocalRotation(entity, a_Rotation);
		}
	}

	void TransformComponentSystem::GetLocalRotations(std::vector<glm::vec3>& a_Rotations,
		const std::vector<Entity>& a_Entities) const
	{
		CROWN_PROFILE_FUNCTION();

		if (a_Rotations.size() != a_Entities.size())
		{
			a_Rotations.resize(a_Entities.size());
		}

		for (size_t i = 0; i < a_Entities.size(); i++)
		{
			a_Rotations[i] = GetLocalRotation(a_Entities[i]);
		}
	}

	//TODO: refactor
	void TransformComponentSystem::SetLookAt(Entity a_Entity, glm::vec3 a_Target)
	{
		auto it = m_Data.find(a_Entity);

		const glm::vec3& position = it->second.localPosition;

		//TODO: use worldPosition
		if (position == a_Target) { return; }

		glm::vec3 direction = glm::normalize(position - a_Target);
		glm::mat4 rotationMatrix = glm::inverse(glm::lookAt(position, position + direction, glm::vec3(0, 1, 0)));
		//Reset position because lookat also includes the position, we don't need it for rotationMatrix
		rotationMatrix[3][0] = 0;
		rotationMatrix[3][1] = 0;
		rotationMatrix[3][2] = 0;

		glm::quat orientation = glm::toQuat(rotationMatrix);
		orientation = glm::conjugate(orientation);

		it->second.localRotation = glm::eulerAngles(orientation);
		//it->second.localRotation.y = -it->second.localRotation.y;
		//it->second.localRotation.x = -it->second.localRotation.x;

		it->second.dirty = true;
	}

	glm::vec3 TransformComponentSystem::GetWorldPosition(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");

		auto it = m_Data.find(a_Entity);

		if (it->second.parent == nullptr)
		{
			return it->second.localPosition;
		}

		return TransformPoint(*it->second.parent, it->second.localPosition);
	}

	glm::vec3 TransformComponentSystem::GetWorldScale(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
		
		auto it = m_Data.find(a_Entity);
		
		if (it->second.parent == nullptr)
		{
			return it->second.localScale;
		}

		return TransformVector(*it->second.parent, it->second.localScale);
	}

	glm::mat4 TransformComponentSystem::CalculateWorldRotationMatrix(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
		
		auto it = m_Data.find(a_Entity);

		glm::vec4 rotation = glm::vec4(it->second.localRotation, 0);

		if (it->second.parent != nullptr)
		{
			rotation = CalculateWorldRotationMatrix(*it->second.parent) * glm::vec4(it->second.localPosition, 0);
		}

		glm::mat4 worldRotationMatrix = glm::rotate(rotation.x, glm::vec3(1, 0, 0));
		worldRotationMatrix = glm::rotate(worldRotationMatrix, rotation.y, glm::vec3(0, 1, 0));
		worldRotationMatrix = glm::rotate(worldRotationMatrix, rotation.z, glm::vec3(0, 0, 1));

		return worldRotationMatrix;
	}

	const glm::mat4& TransformComponentSystem::GetWorldModelMatrix(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Data.count(a_Entity) != 0, "Entity does not exist!");
		
		auto it = m_Data.find(a_Entity);
		
		if (it->second.dirty)
		{
			it->second.worldModelMatrix = glm::translate(GetWorldPosition(a_Entity)) * CalculateWorldRotationMatrix(a_Entity) * glm::scale(GetWorldScale(a_Entity));
			it->second.dirty = false;
		}

		return it->second.worldModelMatrix;
	}

	void TransformComponentSystem::GetWorldModelMatrix(std::vector<glm::mat4>& a_Models, 
		const std::vector<Entity>& a_Entities)
	{
		CROWN_PROFILE_FUNCTION();

		if(a_Models.size() != a_Entities.size())
		{
			a_Models.resize(a_Entities.size());
		}
		
		for (size_t i = 0; i < a_Entities.size(); i++)
		{
			a_Models[i] = GetWorldModelMatrix(a_Entities[i]);
		}
	}
}
