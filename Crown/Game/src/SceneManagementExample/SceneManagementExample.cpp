#include "SceneManagementExample.h"
#include <iostream>

#include <Core/Utilities/Log.h>
#include "Mode.h"
#include "Core/ECS/Entity/Entity.h"
#include "Core/ECS/Component/TransformComponentSystem.h"
#include "Core/ECS/Component/RenderComponentSystem.h"

#include "Core/SceneManagement/SceneManager.h"
#include "Core/SceneManagement/Scene.h"
#include "Core/ECS/Component/TagComponentSystem.h"

#ifdef SCENEEXAMPLE
#include "EntryPoint.h"
Crown::Application* Crown::CreateApplication()
{
	return new SceneManagementExample();
}
#endif

void SceneManagementExample::OnUpdate(float a_DeltaTime)
{

}

void SceneManagementExample::OnRender()
{
	
}

void SceneManagementExample::OnStart()
{
	//Creation of a scene object
	std::shared_ptr<Crown::Scene> scene = Crown::Application::Get().GetSceneManager()->AddScene("TestScene");

	//Adding a custom empty layer to the newly created Scene object
	std::shared_ptr<Crown::SceneLayer> layer1 = scene->AddLayer("TestLayer");

	//Adding a GLTF layer to the newly created Scene object specifying the name of the layer and the path to the GLTF model
	//This will not load the data from the GLTF model in the scene already, that will only happen when the scene is loaded
	std::shared_ptr<Crown::SceneLayer> layer2 = scene->AddGLTFLayer("GLTFLayer1", "assets/models/BlocksWithTexturesSeparate.gltf");

	//Creation of another scene object
	std::shared_ptr<Crown::Scene> scene2 = Crown::Application::Get().GetSceneManager()->AddScene("TestScene2");

	//Adding a GLTF layer to the newly created Scene object specifying the name of the layer and the path to the GLTF model
	//This will not load the data from the GLTF model in the scene already, that will only happen when the scene is loaded
	std::shared_ptr<Crown::SceneLayer> layer3 = scene2->AddGLTFLayer("GLTFLayer2", "assets/models/BlocksWithTexturesSeparate2.gltf");

	//Loading the first Scene
	Crown::Application::Get().GetSceneManager()->LoadScene(0);
	//Loading the second Scene which will first unload the first Scene
	Crown::Application::Get().GetSceneManager()->LoadScene(scene2);
	//Load GLTF model while scene is already loaded
	/*
	 *scene2->AddGLTFEntity("something.gltf", "GLTFLayer2");
	 */
}

void SceneManagementExample::OnLateUpdate(float a_DeltaTime)
{

}

void SceneManagementExample::OnFixedUpdate()
{
	
}

void SceneManagementExample::OnImGuiRender()
{
	
}
