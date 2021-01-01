#include "CrownPCH.h"

#include "Core/Renderer/Models.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/ECS/Component/TagComponentSystem.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/Resource/ResourceManager.h"
#include "Texture.h"
#include "Core/ECS/Component/RenderComponentSystem.h"
#include "Core/ECS/Component/ComponentManager.h"

namespace Crown
{
	Models::Models(const std::vector<std::shared_ptr<ModelData>>& a_ModelDatas)
	{
		for (const auto& modelData : a_ModelDatas)
		{
			//ASSERT_ENGINE(modelData->primitives.size() == 1, "Primitive count must be 1. Multiple primitives is not supported at this moment.");
			//Primitives are treated as if they are their own model (because they are)
			for (auto& primitive : modelData->primitives)
			{
				std::shared_ptr<Model> model = std::make_shared<Model>();
				model->modelData = modelData;
				
				std::vector<float> vertices;

				for (const Vertex& vertex : primitive->vertices)
				{
					vertices.push_back(vertex.pos.x);
					vertices.push_back(vertex.pos.y);
					vertices.push_back(vertex.pos.z);

					vertices.push_back(vertex.normal.x);
					vertices.push_back(vertex.normal.y);
					vertices.push_back(vertex.normal.z);

					vertices.push_back(vertex.tex.x);
					vertices.push_back(vertex.tex.y);
				}

				model->vertexArray = VertexArray::Create(
					vertices.data(),
					static_cast<uint32_t>(vertices.size()),
					primitive->indices.data(),
					static_cast<uint32_t>(primitive->indices.size()));
			
				std::vector<std::shared_ptr<Texture>> textureData;
				const int range = static_cast<int>(TextureTypes::LAST);
				for (int i = 0; i < range; i++)
				{
					std::shared_ptr<Texture> tex = Texture::Create();
					tex->Init();

					switch(i)
					{
						case static_cast<int>(TextureTypes::BASECOLOR) :		//Load base color texture
							tex->Load(primitive->material->baseColorTextureImage);
							break;
						case static_cast<int>(TextureTypes::NORMAL) :			//load normal texture
							tex->Load(primitive->material->normalTextureImage);
							break;
						case static_cast<int>(TextureTypes::OCCLUSION) :		//load occlusion texture
							tex->Load(primitive->material->occlusionTexturImage);
							break;
						case static_cast<int>(TextureTypes::METALLICROUGHNESS) ://load metallic/roughness texture
							tex->Load(primitive->material->metallicRoughnessTextureImage);
							break;
						case static_cast<int>(TextureTypes::EMISSIVE) :			//load emissive texture
							tex->Load(primitive->material->emissiveTextureImage);
							break;
						default:												//load base color by default
							tex->Load(primitive->material->baseColorTextureImage);
							break;
					}

					textureData.push_back(tex);
				}

				model->matData = std::make_shared<Material>();
				//model->matData->color = glm::vec4(1,1,1,1);
				model->matData->color = primitive->material->baseColor;
				model->matData->textureData = textureData;
				m_Models.push_back(model);
				//transform??
			}

			//handle lights and cameras
			
		}
	}

	std::shared_ptr<Models> Models::Load(const std::string& a_Path)
	{
		auto modelDatas = GLTFParser::LoadGLTFMeshesFromPath(a_Path.c_str());

		auto models = std::make_shared<Models>(modelDatas);

		return models;
	}

	std::vector<Entity> Models::LoadModelEntities(const std::string& a_Path)
	{
		auto models = Application::Get().GetResourceManager()->Get<Models>(a_Path);

		std::vector<Entity> entities;

		for (const auto& model : models->GetModels())
		{
			//Create and load scene object
			Entity entity = Entity::Create();
			entities.push_back(entity);

			std::shared_ptr<RenderComponentSystem> renderComponent = entity.AddToSystem<RenderComponentSystem>();
			renderComponent->SetModel(entity, model);

			glm::vec3 position = glm::vec3(
				model->modelData->node.translation[0],
				model->modelData->node.translation[1],
				model->modelData->node.translation[2]);
			
			std::shared_ptr<TransformComponentSystem> transformComponent = entity.AddToSystem<TransformComponentSystem>();
			transformComponent->SetLocalPosition(entity, position);
			
			//Tags
			if (!model->modelData->node.extras.Keys().empty())
			{
				std::shared_ptr<TagComponentSystem> tagComponent = entity.AddToSystem<TagComponentSystem>();
				tagComponent->SetTags(GLTFParser::GetTagsFromNode(model->modelData->node));
			}
		}
		
		return entities;
	}

	std::vector<Entity> Models::LoadModelEntitiesNew(const std::string& a_Path)
	{
		auto models = Application::Get().GetResourceManager()->Get<Models>(a_Path);

		std::vector<Entity> entities;

		for (const auto& model : models->GetModels())
		{
			//Create and load scene object
			Entity entity = Entity::Create();
			entities.push_back(entity);

			glm::vec3 position = glm::vec3(
				model->modelData->node.translation[0],
				model->modelData->node.translation[1],
				model->modelData->node.translation[2]);
			
			TransformComponent transformComponent {
				position
			};
			
			RenderComponent renderComponent
			{
				model,
				glm::mat4(0)
			};

			ComponentManager<RenderComponent>::Add(entity, renderComponent);
			ComponentManager<TransformComponent>::Add(entity, transformComponent);

			//Tags
			if (!model->modelData->node.extras.Keys().empty())
			{
				std::shared_ptr<TagComponentSystem> tagComponent = entity.AddToSystem<TagComponentSystem>();
				tagComponent->SetTags(GLTFParser::GetTagsFromNode(model->modelData->node));
			}
		}

		return entities;
	}

	Entity Models::LoadModelEntity(const std::string& a_Path)
	{
		auto models = Application::Get().GetResourceManager()->Get<Models>(a_Path);

		ASSERT(models->GetModels().size() == 1, "The amount of models loaded in this test should be 1!");

		std::shared_ptr<Crown::Model> model = models->GetModels().at(0);

		Entity entity = Entity::Create();

		glm::vec3 position = glm::vec3(
			model->modelData->node.translation[0],
			model->modelData->node.translation[1],
			model->modelData->node.translation[2]);

		TransformComponent transformComponent{
			position
		};

		RenderComponent renderComponent
		{
			model,
			glm::mat4(0)
		};

		ComponentManager<RenderComponent>::Add(entity, renderComponent);
		ComponentManager<TransformComponent>::Add(entity, transformComponent);

		//Tags
		if (!model->modelData->node.extras.Keys().empty())
		{
			std::shared_ptr<TagComponentSystem> tagComponent = entity.AddToSystem<TagComponentSystem>();
			tagComponent->SetTags(GLTFParser::GetTagsFromNode(model->modelData->node));
		}

		return entity;
	}
}

