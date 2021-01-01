#pragma once

#include "glm/glm.hpp"

namespace Crown
{
	struct TransformComponent;

	class TransformHelper
	{
	public:
		static void SetLocalPosition(TransformComponent& a_TransformComponent, const glm::vec3& a_Position);
		static void TranslateLocal(TransformComponent& a_TransformComponent, const glm::vec3& a_Movement);

		static void SetLocalScale(TransformComponent& a_TransformComponent, const glm::vec3& a_Scale);
		static void ScaleLocal(TransformComponent& a_TransformComponent, const glm::vec3& a_Scaling);

		static void SetLocalRotation(TransformComponent& a_TransformComponent, const glm::vec3& a_Rotation);

		static void SetLookAt(TransformComponent& a_TransformComponent, glm::vec3 a_Target);

		static glm::vec3 InverseTransformPoint(const glm::mat4& a_Model, const glm::vec3& a_WorldPoint); //World to local point 
		static glm::vec3 InverseTransformVector(const glm::mat4& a_Model, const glm::vec3& a_WorldVector); //World to local vector
		static glm::vec3 TransformPoint(const glm::mat4& a_Model, const glm::vec3& a_LocalPoint); //Local to world point
		static glm::vec3 TransformVector(const glm::mat4& a_Model, const glm::vec3& a_LocalVector); //Local to world vector
		
		static glm::vec3 GetRight(TransformComponent& a_TransformComponent) { return TransformVector(GetWorldModelMatrix(a_TransformComponent), glm::vec3(1, 0, 0)); }
		static glm::vec3 GetUp(TransformComponent& a_TransformComponent) { return TransformVector(GetWorldModelMatrix(a_TransformComponent), glm::vec3(0, 1, 0)); }
		static glm::vec3 GetForward(TransformComponent& a_TransformComponent) { return TransformVector(GetWorldModelMatrix(a_TransformComponent), glm::vec3(0, 0, 1)); }

		static glm::vec3 GetWorldPosition(TransformComponent& a_TransformComponent);
		static glm::vec3 GetWorldScale(TransformComponent& a_TransformComponent);
		static glm::mat4 CalculateWorldRotationMatrix(TransformComponent& a_TransformComponent);

		static const glm::mat4& GetWorldModelMatrix(TransformComponent& a_TransformComponent);
	};
}
