#include "CrownPCH.h"
#include "CameraHelper.h"

#include "Core/ECS/Component/ComponentManager.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"

namespace Crown
{
	Entity CameraHelper::s_MainCameraEntity(-1);
	
	CameraComponent CameraHelper::GetMainCameraData()
	{
		ASSERT_ENGINE(s_MainCameraEntity.GetId() != -1, "Main Camera Entity does not exist!");

		return Get(s_MainCameraEntity);
	}

	Entity CameraHelper::GetMainCameraEntity()
	{
		ASSERT_ENGINE(s_MainCameraEntity.GetId() != -1, "Main Camera Entity does not exist!");

		return s_MainCameraEntity;
	}

	CameraComponent CameraHelper::Get(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();

		RecalculateViewMatrix(a_Entity);

		return ComponentManager<CameraComponent>::Get(a_Entity);
	}

	void CameraHelper::SetProjection(Entity a_Entity, float a_AspectRatio, float a_ZoomLevel)
	{
		CROWN_PROFILE_FUNCTION();

		ComponentManager<CameraComponent>::Get(a_Entity).projectionMatrix = glm::ortho(-a_AspectRatio * a_ZoomLevel, a_AspectRatio * a_ZoomLevel, -a_ZoomLevel, a_ZoomLevel);
	}

	void CameraHelper::SetProjection(Entity a_Entity, float a_AspectRatio, float a_Fov, float a_Near, float a_Far)
	{
		CROWN_PROFILE_FUNCTION();

		ComponentManager<CameraComponent>::Get(a_Entity).projectionMatrix = glm::perspective(glm::radians(a_Fov), a_AspectRatio, a_Near, a_Far);
	}

	void CameraHelper::SetCameraData(Entity a_Entity, const CameraData& a_CameraData)
	{
		CROWN_PROFILE_FUNCTION();
		tinygltf::PerspectiveCamera perspectiveCamera = a_CameraData.m_Properties.perspective;

		SetProjection(a_Entity,
			static_cast<float>(perspectiveCamera.aspectRatio),
			static_cast<float>(perspectiveCamera.yfov),
			static_cast<float>(perspectiveCamera.znear),
			static_cast<float>(perspectiveCamera.zfar));

		if (!a_CameraData.m_NodeTransform.translation.empty())
		{
			const glm::vec3 position = glm::vec3(
				a_CameraData.m_NodeTransform.translation.at(0),
				a_CameraData.m_NodeTransform.translation.at(1),
				a_CameraData.m_NodeTransform.translation.at(2));
			TransformHelper::SetLocalPosition(ComponentManager<TransformComponent>::Get(a_Entity), position);
		}
		if (!a_CameraData.m_NodeTransform.rotation.empty())
		{
			const glm::vec3 rotation = glm::vec3(
				a_CameraData.m_NodeTransform.rotation.at(0),
				a_CameraData.m_NodeTransform.rotation.at(1),
				a_CameraData.m_NodeTransform.rotation.at(2));
			TransformHelper::SetLocalRotation(ComponentManager<TransformComponent>::Get(a_Entity), rotation);
		}
	}

	void Crown::CameraHelper::SetProjection(Entity a_Entity, const glm::mat4& a_ProjectionMatrix)
	{
		CROWN_PROFILE_FUNCTION();

		ComponentManager<CameraComponent>::Get(a_Entity).projectionMatrix = a_ProjectionMatrix;
	}

	void Crown::CameraHelper::RecalculateViewMatrix(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();

		CameraComponent cameraComponent = ComponentManager<CameraComponent>::Get(a_Entity);
		
		cameraComponent.viewMatrix = glm::inverse(TransformHelper::GetWorldModelMatrix(ComponentManager<TransformComponent>::Get(a_Entity)));
		cameraComponent.viewProjectionMatrix = cameraComponent.projectionMatrix * cameraComponent.viewMatrix;

		ComponentManager<CameraComponent>::Set(a_Entity, cameraComponent);
	}

}
