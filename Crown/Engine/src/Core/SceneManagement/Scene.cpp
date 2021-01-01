#include "CrownPCH.h"
#include "Scene.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"
#include "Core/ECS/Component/TagComponentSystem.h"
#include "Core/Resource/ResourceManager.h"
#include "Core/SceneManagement/SceneManager.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Models.h"
#include "Core/ECS/Component/CameraComponentSystem.h"

Crown::Scene::Scene(std::string a_Name, unsigned int a_SceneID)
{
	CROWN_PROFILE_FUNCTION();
	//Setting member variables
	m_Name = a_Name;
	m_SceneID = a_SceneID;

	//Creating a default layer
	m_Layers.push_back(std::make_shared<SceneLayer>("Default"));
}

Crown::Scene::~Scene()
{
	
}

std::shared_ptr<Crown::SceneLayer> Crown::Scene::AddLayer(std::string a_LayerName)
{
	CROWN_PROFILE_FUNCTION();

	std::shared_ptr<SceneLayer> layer = std::make_shared<SceneLayer>(a_LayerName);
	m_Layers.push_back(layer);
	return layer;
}

std::shared_ptr<Crown::SceneLayer> Crown::Scene::AddGLTFLayer(std::string a_LayerName, std::string a_PathToGLTFScene)
{
	CROWN_PROFILE_FUNCTION();

	std::shared_ptr<SceneLayer> layer = std::make_shared<SceneLayer>(a_LayerName);
	layer->pathToGLTF = a_PathToGLTFScene;
	m_Layers.push_back(layer);
	return layer;
}

void Crown::Scene::RemoveLayer(std::string a_LayerName)
{
	CROWN_PROFILE_FUNCTION();

	ASSERT_ENGINE(a_LayerName != "", "Cannot remove a layer with no name");
	///First get the layer with the specified name, then remove all it's entities
	std::shared_ptr<SceneLayer> layer = GetSceneLayerByName(a_LayerName);
	ASSERT_ENGINE(layer != nullptr, "Could not find layer with name " << a_LayerName);

	size_t size = layer->entities.size();

	for (size_t i = 0; i < size; i++)
	{
		RemoveEntity(layer->entities[0]);
	}
}

void Crown::Scene::AddGLTFEntity(std::string a_PathToGLTF)
{
	ASSERT_ENGINE(a_PathToGLTF != "", "Empty GLTF file is trying to be loaded");

	ConstructObjectsFromGLTF(a_PathToGLTF, m_Layers[0]);
}

void Crown::Scene::AddGLTFEntity(std::string a_PathToGLTF, std::string a_LayerName)
{
	ASSERT_ENGINE(a_PathToGLTF != "", "Empty GLTF file is trying to be loaded");
	ASSERT_ENGINE(a_LayerName != "", "Layer to push this GLTF file in is empty");

	std::shared_ptr<SceneLayer> layer = GetSceneLayerByName(a_LayerName);
	ConstructObjectsFromGLTF(a_PathToGLTF, layer);
}

Crown::Entity& Crown::Scene::AddEntity()
{
	CROWN_PROFILE_FUNCTION();

	//Creating entity and pushing the entity in the default layer which is the layer with index 0
	Entity entity = Entity::Create();
	m_Layers.at(0)->entities.push_back(entity);
	return m_Layers.at(0)->entities[m_Layers.at(0)->entities.size() - 1];
}

Crown::Entity& Crown::Scene::AddEntity(std::string a_LayerName)
{
	CROWN_PROFILE_FUNCTION();
	
	ASSERT_ENGINE(a_LayerName != "", "Trying to add an entity to an empty layer name");
	///First get the layer with the specified name
	std::shared_ptr<SceneLayer> layer = GetSceneLayerByName(a_LayerName);
	ASSERT_ENGINE(layer != nullptr, "Could not find layer with name " << a_LayerName);
	///Then push back the entity to create to the layer we just found
	Entity entity = Entity::Create();
	layer->entities.push_back(entity);
	return layer->entities[layer->entities.size() - 1];
}

void Crown::Scene::RemoveEntity(Entity& a_Entity)
{
	CROWN_PROFILE_FUNCTION();

	bool found = false;

	//For every layer
	for(auto layer : m_Layers)
	{
		//Only remove the entity when not found yet
		if(!found)
		{
			//Search for the entity to remove in this layer
			auto it = std::find(layer->entities.begin(), layer->entities.end(), a_Entity);

			if (it != layer->entities.end())
			{
				it->RemoveAllComponents();
				layer->entities.erase(it);
				found = true;
			}
		}
	}
}

std::shared_ptr<Crown::SceneLayer> Crown::Scene::GetSceneLayerByName(std::string a_Name)
{
	CROWN_PROFILE_FUNCTION();

	for (auto& layer : m_Layers)
	{
		if (layer->name == a_Name)
		{
			return layer;
		}
	}
	ASSERT_ENGINE(a_Name != a_Name, "Layer is not found in this current scene");
	return nullptr;
}

std::string Crown::Scene::GetName()
{
	return m_Name;
}

int Crown::Scene::GetID()
{
	return m_SceneID;
}


std::vector<std::shared_ptr<Crown::SceneLayer>>Crown::Scene::GetLayers()
{
	return m_Layers;
}

void Crown::Scene::Load()
{
	CROWN_PROFILE_FUNCTION();

	for(auto layer : m_Layers)
	{
		if(layer->pathToGLTF != "")
		{
			ConstructObjectsFromGLTF(layer->pathToGLTF, layer);
		}
	}
}

void Crown::Scene::UnLoad()
{
	CROWN_PROFILE_FUNCTION();

	for(auto layer : m_Layers)
	{
		RemoveLayer(layer->name);
	}
}

void Crown::Scene::ConstructObjectsFromGLTF(std::string a_PathToGLTF, std::shared_ptr<SceneLayer> a_Layer)
{
	CROWN_PROFILE_FUNCTION();

	auto entities = Models::LoadModelEntities(a_PathToGLTF);

	for (const auto& entity : entities)
	{
		a_Layer->entities.push_back(entity);
	}

	//Parse all cameras
	auto cameras = GLTFParser::LoadGLTFCamerasFromPath(a_PathToGLTF.c_str());
	//Parse all lights
	auto lights = GLTFParser::LoadGLTFLightsFromPath(a_PathToGLTF.c_str());

	for (auto camera : cameras)
	{
		m_Cameras.push_back(camera);
		Entity cameraEntity = Entity::Create();
		cameraEntity.AddToSystem<TransformComponentSystem>();
		auto cameraComponentSystem = cameraEntity.AddToSystem<CameraComponentSystem>();

		cameraComponentSystem->SetCameraData(cameraEntity, *camera);
		cameraComponentSystem->SetMainCameraEntity(cameraEntity);
	}

	if(cameras.empty())
	{
		Entity cameraEntity = Entity::Create();
		cameraEntity.AddToSystem<TransformComponentSystem>();
		auto cameraComponentSystem = cameraEntity.AddToSystem<CameraComponentSystem>();

		cameraComponentSystem->SetProjection(cameraEntity, 1.6f, 60.f, 0.1f, 100);
		cameraComponentSystem->SetMainCameraEntity(cameraEntity);
	}
}