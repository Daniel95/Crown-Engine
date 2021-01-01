#include "CrownPCH.h"
#include "SceneManager.h"

Crown::SceneManager::SceneManager()
{
	m_Index = 0;
}

Crown::SceneManager::~SceneManager()
{

}

std::shared_ptr<Crown::Scene> Crown::SceneManager::AddScene(std::string a_SceneName)
{
	ASSERT_ENGINE(a_SceneName != "", "Trying to create a scene with an empty name");

	//Is index 0 in scenes already?
	if (m_Scenes.size() != 0)
	{
		m_Index++;
	}

	std::shared_ptr<Scene> scene = std::make_shared<Scene>(a_SceneName, m_Index);
	m_Scenes.push_back(scene);
	return scene;
}


std::shared_ptr<Crown::Scene> Crown::SceneManager::LoadScene(std::shared_ptr<Scene> a_Scene)
{
	ASSERT_ENGINE(a_Scene != nullptr, "Trying to load an empty scene");
	
	std::shared_ptr<Scene> shouldLoadScene = nullptr;

	for(auto& scene : m_Scenes)
	{
		if(scene->GetName() == a_Scene->GetName())
		{
			shouldLoadScene = scene;
		}
	}

	ASSERT_ENGINE(shouldLoadScene != nullptr, "Cant find the scene with name: " << a_Scene->GetName());

	if(m_CurrentScene)
	{
		m_CurrentScene->UnLoad();
	}

	m_CurrentScene = shouldLoadScene;
	m_CurrentScene->Load();
	return m_CurrentScene;
}

std::shared_ptr<Crown::Scene> Crown::SceneManager::LoadScene(std::string a_SceneName)
{
	ASSERT_ENGINE(a_SceneName != "", "Trying to load a scene with no name");

	std::shared_ptr<Scene> shouldLoadScene = nullptr;

	//Find scene with name
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == a_SceneName)
		{
			shouldLoadScene = scene;
		}
	}

	ASSERT_ENGINE(shouldLoadScene != nullptr, "Cant find the scene with name: " << a_SceneName);

	if (m_CurrentScene)
	{
		m_CurrentScene->UnLoad();
	}

	m_CurrentScene = shouldLoadScene;
	m_CurrentScene->Load();
	return m_CurrentScene;
}

std::shared_ptr<Crown::Scene> Crown::SceneManager::LoadScene(unsigned int a_SceneID)
{
	std::shared_ptr<Scene> shouldLoadScene = nullptr;

	//Find scene with ID
	for (auto& scene : m_Scenes)
	{
		if (scene->GetID() == a_SceneID)
		{
			shouldLoadScene = scene;
		}
	}

	ASSERT_ENGINE(shouldLoadScene != nullptr, "Cant find the scene with id: " << a_SceneID);

	if (m_CurrentScene)
	{
		m_CurrentScene->UnLoad();
	}

	m_CurrentScene = shouldLoadScene;
	m_CurrentScene->Load();
	return m_CurrentScene;
}

void Crown::SceneManager::RemoveScene(std::shared_ptr<Scene> a_Scene)
{
	ASSERT_ENGINE(a_Scene != nullptr, "Scene to remove is nullptr");
	auto it = std::find(m_Scenes.begin(), m_Scenes.end(), a_Scene);
	ASSERT_ENGINE(it != m_Scenes.end(), "Scene not found in scenes vector");
	//Unload found scene first
	(*it)->UnLoad();
	if(m_CurrentScene == (*it))
	{
		m_CurrentScene = nullptr;
	}
	m_Scenes.erase(it);
}

void Crown::SceneManager::RemoveScene(std::string a_SceneName)
{
	std::shared_ptr<Scene> shouldLoadScene = nullptr;

	//Find scene with name
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == a_SceneName)
		{
			shouldLoadScene = scene;
		}
	}

	ASSERT_ENGINE(shouldLoadScene != nullptr, "Cant find the scene with name: " << a_SceneName);

	//Unload scene first
	shouldLoadScene->UnLoad();
	//Remove the scene
	RemoveScene(shouldLoadScene);
}

void Crown::SceneManager::RemoveScene(unsigned a_SceneID)
{
	std::shared_ptr<Scene> shouldLoadScene = nullptr;

	//Find scene with ID
	for (auto& scene : m_Scenes)
	{
		if (scene->GetID() == a_SceneID)
		{
			shouldLoadScene = scene;
		}
	}

	ASSERT_ENGINE(shouldLoadScene != nullptr, "Cant find the scene with id: " << a_SceneID);

	//Unload scene first
	shouldLoadScene->UnLoad();
	//Remove the scene
	RemoveScene(shouldLoadScene);
}

std::shared_ptr<Crown::Scene> Crown::SceneManager::GetSceneByName(const std::string& a_SceneName)
{
	ASSERT_ENGINE(a_SceneName != "", "Trying to get a scene by an empty name");
	std::shared_ptr<Scene> returnScene = nullptr;
	for (auto& scene : m_Scenes)
	{
		if (scene->GetName() == a_SceneName)
		{
			returnScene = scene;
		}
	}
	
	ASSERT_ENGINE(returnScene != nullptr, "Cant find the scene with name: " << a_SceneName);
	return returnScene;
	
}

std::shared_ptr<Crown::Scene> Crown::SceneManager::GetSceneByID(unsigned a_ID)
{
	std::shared_ptr<Scene> returnScene = nullptr;
	for (auto& scene : m_Scenes)
	{
		if (scene->GetID() == a_ID)
		{
			returnScene = scene;
		}
	}

	ASSERT_ENGINE(returnScene != nullptr, "Cant find the scene with id: " << a_ID);
	return returnScene;
}

std::shared_ptr<Crown::Scene> Crown::SceneManager::GetActiveScene()
{
	return m_CurrentScene;
}

std::vector<std::shared_ptr<Crown::Scene>> Crown::SceneManager::GetScenes()
{
	return m_Scenes;
}
