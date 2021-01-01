#pragma once
#include "Core/ECS/System/ComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
	struct CameraData;
	class TransformComponentSystem;
	
	struct CameraComponent
	{
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		glm::mat4 projectionMatrix = glm::mat4(1.0f);
		glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);
		float zoomLevel = 1.0f;
		float aspectRatio = 1.6f;
	};
		
	class CameraComponentSystem : public ComponentSystem
	{
	public:
		CameraComponentSystem();
		~CameraComponentSystem() = default;
		
		virtual void OnAddEntity(Entity a_Entity) override;
		virtual void OnRemoveEntity(Entity a_Entity) override { m_Components.erase(a_Entity); }

		CameraComponent GetMainCameraData();
		static Entity GetMainCameraEntity();
		static void SetMainCameraEntity(Entity a_Entity) { s_MainCameraEntity = a_Entity; }
		
		CameraComponent Get(Entity a_Entity);
		void SetProjection(Entity a_Entity, float a_AspectRatio, float a_ZoomLevel);
		void SetProjection(Entity a_Entity, float a_AspectRatio, float a_Fov, float a_Near, float a_Far);
		//void SetProjection(Entity a_Entity, float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0f, float a_Far = 1.0f);
		void SetCameraData(Entity a_Entity, const CameraData& a_CameraData);
		void SetProjection(Entity a_Entity, const glm::mat4& a_ProjectionMatrix);
		void RecalculateViewMatrix(Entity a_Entity);

	private:
		std::unordered_map<int, CameraComponent> m_Components;
		std::shared_ptr<TransformComponentSystem> m_TransformComponentSystem;

		static Entity s_MainCameraEntity;
	};
}
