#pragma once

#include "Core/ECS/System/ComponentSystem.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <memory>
#include <unordered_map>

namespace Crown
{
	class VertexArray;
	class Texture;
	class TransformComponentSystem;
	class Entity;
	struct Model;
	struct Material;

	struct RenderComponent
	{
		std::shared_ptr<Model> model;
		//std::shared_ptr<Material> material;
		glm::mat4 modelMatrix;
	};
	
	class RenderComponentSystem : public ComponentSystem
	{
	public:
		RenderComponentSystem();
		~RenderComponentSystem() = default;

		virtual void OnUpdate(float a_DeltaTime) override;
		virtual void OnAddEntity(Entity a_Entity) override;
		virtual void OnRemoveEntity(Entity a_Entity) override;
		void SetModel(Entity a_Entity, const std::shared_ptr<Model>& a_Model);
		//void SetMat(Entity a_Entity, const std::shared_ptr<Material>& a_Material);

	private:
		std::unordered_map<int, RenderComponent> m_Data;
		std::shared_ptr<TransformComponentSystem> m_Transform;
		
	};
}
