#include "CrownPCH.h"
#include "SceneParser.h"

std::shared_ptr<Crown::SceneParseData> Crown::SceneParser::LoadSceneFromFile(const char* a_Filename)
{
    //The actual scene data object
	std::shared_ptr<SceneParseData> sceneData = std::make_shared<SceneParseData>();

    //Model hierarchy of the gltf scene that is loaded in
	std::vector<std::shared_ptr<ModelData>> modelHierarchy = GLTFParser::LoadGLTFMeshesFromPath(a_Filename);

    //Parse and get models
	for (size_t i = 0; i < modelHierarchy.size(); ++i)
	{
		//Create and load scene object
		std::shared_ptr<SceneObject> sceneObject = std::make_shared<SceneObject>();;
		sceneObject->m_Name = modelHierarchy[i]->m_Name;
		sceneObject->m_ModelData = modelHierarchy[i];

		//Assign children
		CreateRecursiveChildren(sceneData, sceneObject, modelHierarchy[i]);

        //Lastly add scene object to scene object vector
		sceneData->m_SceneObjects.push_back(sceneObject);
	}

    //Parse all cameras
	sceneData->m_Cameras = GLTFParser::LoadGLTFCamerasFromPath(a_Filename);
    //Parse all lights
    sceneData->m_Lights = GLTFParser::LoadGLTFLightsFromPath(a_Filename);

	return sceneData;
}

void Crown::SceneParser::CreateRecursiveChildren(std::shared_ptr<SceneParseData> a_Scene, std::shared_ptr<SceneObject> a_Obj, std::shared_ptr<ModelData> a_ModelData)
{
	for (size_t i = 0; i < a_ModelData->m_Children.size(); i++)
	{
		///Create child scene object and assign child data
		std::shared_ptr<SceneObject> sceneObject = std::make_shared<SceneObject>();
		sceneObject->m_Name = a_ModelData->m_Children[i]->m_Name;
		sceneObject->m_ModelData = a_ModelData->m_Children[i];
		a_Scene->m_SceneObjects.push_back(sceneObject);

		//Set child <-> parent relationship
		a_Obj->m_Children.push_back(sceneObject);
		sceneObject->m_Parent = a_Obj;

		//Try to search for another child in this current child recursively
		CreateRecursiveChildren(a_Scene, sceneObject, a_ModelData->m_Children[i]);
	}
}
