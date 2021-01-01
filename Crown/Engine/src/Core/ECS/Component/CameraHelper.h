#pragma once
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/CameraComponentSystem.h"
#include "Core/ECS/Entity/Entity.h"

namespace Crown
{
	struct CameraData;
	
	class CameraHelper
	{
	public:
		static Entity s_MainCameraEntity;

		static CameraComponent GetMainCameraData();
		static Entity GetMainCameraEntity();
		static void SetMainCameraEntity(Entity a_Entity) { s_MainCameraEntity = a_Entity; }

		static CameraComponent Get(Entity a_Entity);
		static void SetProjection(Entity a_Entity, float a_AspectRatio, float a_ZoomLevel);
		static void SetProjection(Entity a_Entity, float a_AspectRatio, float a_Fov, float a_Near, float a_Far);
		//void SetProjection(Entity a_Entity, float a_Left, float a_Right, float a_Bottom, float a_Top, float a_Near = -1.0f, float a_Far = 1.0f);
		static void SetCameraData(Entity a_Entity, const CameraData& a_CameraData);
		static void SetProjection(Entity a_Entity, const glm::mat4& a_ProjectionMatrix);
		static void RecalculateViewMatrix(Entity a_Entity);

	};
}
