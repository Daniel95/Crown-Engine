#pragma once
#include "../Renderer/GLTFLoading/GLTFParser.h"

namespace Crown
{
    //Temp object file
    class SceneObject
    {
    public:
        //Name of this object
		std::string m_Name;
        //Tag of this object
		const char* m_Tag;
        //Model data of this object
		std::shared_ptr<ModelData> m_ModelData = nullptr;
        //Parent object of this object, can be nullptr if no parent exists
		std::shared_ptr<SceneObject> m_Parent;
        //Child objects of this object, can be an empty vector if no children exists
		std::vector<std::shared_ptr<SceneObject>> m_Children;
    };

    //Struct parse data for a whole fx gltf scene
    struct SceneParseData
    {
    public:
        //All scene objects from this scene
		std::vector<std::shared_ptr<SceneObject>> m_SceneObjects;
        //All cameras in the scene
		std::vector<std::shared_ptr<CameraData>> m_Cameras;
        //All lights in the scene
		std::vector<std::shared_ptr<LightData>> m_Lights;
        //All corresponding model data objects from this scene (used for calculating scene objects of this scene)
		std::vector<std::shared_ptr<ModelData>> m_ModelDataObjects;
    };

    //Class responsible for parsing and loading an entire fx gltf scene
	class SceneParser
	{
	public:
        //Returns an entire fully loaded fx gltf scene from an file path
		static std::shared_ptr<SceneParseData> LoadSceneFromFile(const char* a_Filename);

    private:
        //Searches for new children objects to parse based on model data from another object
		static void CreateRecursiveChildren(std::shared_ptr<SceneParseData> a_Scene, std::shared_ptr<SceneObject> a_Obj, std::shared_ptr<ModelData> a_ModelData);
	};
}

