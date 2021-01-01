#include "CrownPCH.h"
#include "TransformHelper.h"

#include "TransformComponentSystem.h"

namespace Crown
{
	void TransformHelper::SetLocalPosition(TransformComponent& a_TransformComponent, const glm::vec3& a_Position)
	{
		CROWN_PROFILE_FUNCTION();

		a_TransformComponent.dirty = true;
		a_TransformComponent.localPosition = a_Position;
	}

	void TransformHelper::TranslateLocal(TransformComponent& a_TransformComponent, const glm::vec3& a_Movement)
	{
		CROWN_PROFILE_FUNCTION();

		a_TransformComponent.dirty = true;
		a_TransformComponent.localPosition += a_Movement;
	}

	void TransformHelper::SetLocalScale(TransformComponent& a_TransformComponent, const glm::vec3& a_Scale)
	{
		CROWN_PROFILE_FUNCTION();

		a_TransformComponent.dirty = true;
		a_TransformComponent.localScale = a_Scale;
	}

	void TransformHelper::ScaleLocal(TransformComponent& a_TransformComponent, const glm::vec3& a_Scaling)
	{
		CROWN_PROFILE_FUNCTION();

		a_TransformComponent.dirty = true;
		a_TransformComponent.localScale += a_Scaling;
	}

	void TransformHelper::SetLocalRotation(TransformComponent& a_TransformComponent, const glm::vec3& a_Rotation)
	{
		CROWN_PROFILE_FUNCTION();

		a_TransformComponent.dirty = true;
		a_TransformComponent.localRotation = a_Rotation;
	}

	void TransformHelper::SetLookAt(TransformComponent& a_TransformComponent, glm::vec3 a_Target)
	{
		CROWN_PROFILE_FUNCTION();

		const glm::vec3& position = a_TransformComponent.localPosition;

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

		a_TransformComponent.localRotation = glm::eulerAngles(orientation);
		a_TransformComponent.localRotation.y = -a_TransformComponent.localRotation.y;
		a_TransformComponent.localRotation.x = -a_TransformComponent.localRotation.x;

		a_TransformComponent.dirty = true;
	}

	glm::vec3 TransformHelper::InverseTransformPoint(const glm::mat4& a_Model, const glm::vec3& a_WorldPoint)
	{
		CROWN_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(a_Model) * glm::vec4(a_WorldPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformHelper::InverseTransformVector(const glm::mat4& a_Model, const glm::vec3& a_WorldVector)
	{
		CROWN_PROFILE_FUNCTION();

		const glm::vec4 transformed = glm::inverse(a_Model) * glm::vec4(a_WorldVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformHelper::TransformPoint(const glm::mat4& a_Model, const glm::vec3& a_LocalPoint)
	{
		CROWN_PROFILE_FUNCTION();

		const glm::vec4 transformed = a_Model * glm::vec4(a_LocalPoint, 1);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformHelper::TransformVector(const glm::mat4& a_Model, const glm::vec3& a_LocalVector)
	{
		CROWN_PROFILE_FUNCTION();

		const glm::vec4 transformed = a_Model * glm::vec4(a_LocalVector, 0);
		return glm::vec3(transformed);
	}

	glm::vec3 TransformHelper::GetWorldPosition(TransformComponent& a_TransformComponent)
	{
		CROWN_PROFILE_FUNCTION();

		//if (a_TransformComponent.parent == nullptr)
		//{
			return a_TransformComponent.localPosition;
		//}

		//return TransformPoint(*a_TransformComponent.parent, a_TransformComponent.localPosition);
	}

	glm::vec3 TransformHelper::GetWorldScale(TransformComponent& a_TransformComponent)
	{
		CROWN_PROFILE_FUNCTION();

		//if (a_TransformComponent.parent == nullptr)
		//{
			return a_TransformComponent.localScale;
		//}

		//return TransformVector(*a_TransformComponent.parent, a_TransformComponent.localScale);
	}

	glm::mat4 TransformHelper::CalculateWorldRotationMatrix(TransformComponent& a_TransformComponent)
	{
		CROWN_PROFILE_FUNCTION();

		glm::vec4 rotation = glm::vec4(a_TransformComponent.localRotation, 0);

		/*
		if (a_TransformComponent.parent != nullptr)
		{
			rotation = CalculateWorldRotationMatrix(*a_TransformComponent.parent) * glm::vec4(a_TransformComponent.localPosition, 0);
		}
		*/

		glm::mat4 worldRotationMatrix = glm::rotate(rotation.x, glm::vec3(1, 0, 0));
		worldRotationMatrix = glm::rotate(worldRotationMatrix, rotation.y, glm::vec3(0, 1, 0));
		worldRotationMatrix = glm::rotate(worldRotationMatrix, rotation.z, glm::vec3(0, 0, 1));

		return worldRotationMatrix;
	}

	const glm::mat4& TransformHelper::GetWorldModelMatrix(TransformComponent& a_TransformComponent)
	{
		CROWN_PROFILE_FUNCTION();

		if (a_TransformComponent.dirty)
		{
			a_TransformComponent.worldModelMatrix = glm::translate(GetWorldPosition(a_TransformComponent)) * CalculateWorldRotationMatrix(a_TransformComponent) * glm::scale(GetWorldScale(a_TransformComponent));
			a_TransformComponent.dirty = false;
		}

		return a_TransformComponent.worldModelMatrix;
	}
}
