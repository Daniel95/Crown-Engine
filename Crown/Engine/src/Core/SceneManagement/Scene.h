#pragma once
#include "Core/ECS/Entity/Entity.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"

namespace Crown {
	struct SceneLayer
	{
		std::string name;
		std::string pathToGLTF;
		std::vector<Crown::Entity> entities;

		SceneLayer(std::string a_Name) : name(a_Name) {}
	};
	
	class Scene
	{
	public:
		Scene(std::string a_Name, unsigned int a_SceneID);
		~Scene();
		
		//Adds a layer to this scene providing a name to that layer
		std::shared_ptr<SceneLayer> AddLayer(std::string a_LayerName);
		//Adds a GLTF layer to this scene providing not only the name to that layer but also a path to the GLTF model that needs to be loaded
		std::shared_ptr<SceneLayer> AddGLTFLayer(std::string a_LayerName, std::string a_PathToGLTFScene);
		//Removed a layer with a specific name
		void RemoveLayer(std::string a_LayerName);

		//Add single GLTF file into the standard layer
		void AddGLTFEntity(std::string a_PathToGLTF);
		//Add single GLTF file into a layer with a name
		void AddGLTFEntity(std::string a_PathToGLTF, std::string a_LayerName);
		
		//Adds an entity to the default "Default" layer
		Entity& AddEntity();
		//Adds an entity to a layer specified with a layer name
		Entity& AddEntity(std::string a_LayerName);
		//Removes an entity without caring which layer this entity is in
		void RemoveEntity(Entity& a_Entity);

		///Various getter functions
		std::shared_ptr<SceneLayer> GetSceneLayerByName(std::string a_Name);
		std::string GetName();
		int GetID();
		std::vector<std::shared_ptr<SceneLayer>> GetLayers();
				
	private:
		//Loads every layer of this scene
		void Load();
		//Unloads every layer of this scene
		void UnLoad();
		//Construct objects from loaded GLTF model data
		void ConstructObjectsFromGLTF(std::string a_PathToGLTF, std::shared_ptr<SceneLayer> a_Layer);

		friend class SceneManager;
		
	private:
		///Layer name and ID
		std::string m_Name;
		unsigned int m_SceneID;

		//All layers in scene
		std::vector<std::shared_ptr<SceneLayer>> m_Layers;
		//All cameras in the scene
		std::vector<std::shared_ptr<CameraData>> m_Cameras;
        //All lights in the scene
		std::vector<std::shared_ptr<LightData>> m_Lights;
	};
}
