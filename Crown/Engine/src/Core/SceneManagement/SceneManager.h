#pragma once
#include "Core/SceneManagement/Scene.h"

namespace Crown
{
	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		//Adds a scene to all the scenes in this manager, give a scene name
		std::shared_ptr<Scene> AddScene(std::string a_SceneName);

		//Loads a scene based on a pointer to the scene needed to load
		std::shared_ptr<Scene> LoadScene(std::shared_ptr<Scene> a_Scene);
		//Loads a scene based on a name of the scene needed to load
		std::shared_ptr<Scene> LoadScene(std::string a_SceneName);
		//Loads a scene based on the unique id of the scene needed to load
		std::shared_ptr<Scene> LoadScene(unsigned int a_SceneID);

		//Removes a scene based on a pointer to the scene needed to be removed
		void RemoveScene(std::shared_ptr<Scene> a_Scene);
		//Removes a scene based on a name to the scene needed to be removed
		void RemoveScene(std::string a_SceneName);
		//Remove as scene based on a unique ID of the scene needed to be removed
		void RemoveScene(unsigned int a_SceneID);

		std::shared_ptr<Scene> GetSceneByName(const std::string& a_SceneName);
		std::shared_ptr<Scene> GetSceneByID(unsigned int a_ID);

		//Returns the current active scene
		std::shared_ptr<Scene> GetActiveScene();
		//Returns all scene
		std::vector<std::shared_ptr<Scene>> GetScenes();

	private:
		//Used for giving scenes a unique number
		unsigned int m_Index;
		
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_CurrentScene;
	};
}
