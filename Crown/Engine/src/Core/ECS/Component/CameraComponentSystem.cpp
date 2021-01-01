#include "CrownPCH.h"
#include "CameraComponentSystem.h"
#include "TransformComponentSystem.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"

namespace Crown
{
	Entity CameraComponentSystem::s_MainCameraEntity(-1);
	
	CameraComponentSystem::CameraComponentSystem()
	{
		ASSERT_ENGINE(Application::Get().GetComponentSystemManager()->GetSystem<TransformComponentSystem>() != nullptr, "TransformComponentSystem ComponentSystem does not exists yet, create TransformComponentSystem before creating the CameraComponentSystem ComponentSystem.");

		m_TransformComponentSystem = Application::Get().GetComponentSystemManager()->GetSystem<TransformComponentSystem>();
	}

	void CameraComponentSystem::OnAddEntity(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();

		std::pair<int, CameraComponent> pair { a_Entity, {} };
		m_Components.insert(pair);
	}

	CameraComponent CameraComponentSystem::GetMainCameraData()
	{
		ASSERT_ENGINE(s_MainCameraEntity.GetId() != -1, "Main Camera Entity does not exist!");
		
		return Get(s_MainCameraEntity);
	}

	Entity CameraComponentSystem::GetMainCameraEntity()
	{
		ASSERT_ENGINE(s_MainCameraEntity.GetId() != -1, "Main Camera Entity does not exist!");

		return s_MainCameraEntity;
	}

	void CameraComponentSystem::SetProjection(Entity a_Entity, float a_AspectRatio, float a_Fov,
											float a_Near, float a_Far)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Components.count(a_Entity) != 0, "Entity does not exist!");

		m_Components.find(a_Entity)->second.projectionMatrix = glm::perspective(glm::radians(a_Fov), a_AspectRatio, a_Near, a_Far);
	}

	void CameraComponentSystem::SetCameraData(Entity a_Entity, const CameraData& a_CameraData)
	{
		tinygltf::PerspectiveCamera perspectiveCamera = a_CameraData.m_Properties.perspective;
		
		SetProjection(a_Entity, 
			static_cast<float>(perspectiveCamera.aspectRatio), 
			static_cast<float>(perspectiveCamera.yfov),
			static_cast<float>(perspectiveCamera.znear), 
			static_cast<float>(perspectiveCamera.zfar));

		if(!a_CameraData.m_NodeTransform.translation.empty())
		{
			const glm::vec3 position = glm::vec3(
				a_CameraData.m_NodeTransform.translation.at(0),
				a_CameraData.m_NodeTransform.translation.at(1), 
				a_CameraData.m_NodeTransform.translation.at(2));
			m_TransformComponentSystem->SetLocalPosition(a_Entity, position);
		}
		if (!a_CameraData.m_NodeTransform.rotation.empty())
		{
			const glm::vec3 rotation = glm::vec3(
				a_CameraData.m_NodeTransform.rotation.at(0),
				a_CameraData.m_NodeTransform.rotation.at(1), 
				a_CameraData.m_NodeTransform.rotation.at(2));

			m_TransformComponentSystem->SetLocalRotation(a_Entity, rotation);
		}
	}

	CameraComponent CameraComponentSystem::Get(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Components.count(a_Entity) != 0, "Entity does not exist!");

		RecalculateViewMatrix(a_Entity);

		return m_Components.find(a_Entity)->second;
	}

	void CameraComponentSystem::SetProjection(Entity a_Entity, float a_AspectRatio, float a_ZoomLevel)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Components.count(a_Entity) != 0, "Entity does not exist!");

		m_Components.find(a_Entity)->second.projectionMatrix = glm::ortho(-a_AspectRatio * a_ZoomLevel, a_AspectRatio * a_ZoomLevel, -a_ZoomLevel, a_ZoomLevel);
	}

	/*
	void CameraComponentSystem::SetProjection(Entity a_Entity, float a_Left, float a_Right, float a_Bottom, float a_Top,
		float a_Near, float a_Far)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Components.count(a_Entity) != 0, "Entity does not exist!");
		
		m_Components.find(a_Entity)->second->projectionMatrix = glm::ortho(a_Left, a_Right, a_Bottom, a_Top, a_Near, a_Far);
	}
	*/

	void CameraComponentSystem::SetProjection(Entity a_Entity, const glm::mat4& a_ProjectionMatrix)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Components.count(a_Entity) != 0, "Entity does not exist!");
		
		m_Components.find(a_Entity)->second.projectionMatrix = a_ProjectionMatrix;
	}

	void CameraComponentSystem::RecalculateViewMatrix(Entity a_Entity)
	{
		CROWN_PROFILE_FUNCTION();
		ASSERT_ENGINE(m_Components.count(a_Entity) != 0, "Entity does not exist!");

		auto it = m_Components.find(a_Entity);

		it->second.viewMatrix = glm::inverse(m_TransformComponentSystem->GetWorldModelMatrix(a_Entity));
		it->second.viewProjectionMatrix = it->second.projectionMatrix * it->second.viewMatrix;
	}
}