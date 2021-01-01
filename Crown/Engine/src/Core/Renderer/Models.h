#pragma once
#include "Core/ECS/Entity/Entity.h"
#include "Core/Resource/Resource.h"

#include <vector>

namespace Crown
{
	class Texture;
	class VertexArray;

	enum class TextureTypes
	{
		BASECOLOR = 0,
		NORMAL = 1,
		OCCLUSION = 2,
		METALLICROUGHNESS = 3,
		EMISSIVE = 4,
		FIRST = BASECOLOR,
		LAST = EMISSIVE, 
	};

	struct Material
	{
		std::vector<std::shared_ptr<Texture>> textureData;
		glm::vec4 color = glm::vec4(1);
	};
	
	struct Model
	{
		std::shared_ptr<VertexArray> vertexArray;
		std::shared_ptr<ModelData> modelData;
		std::shared_ptr<Material> matData;
	};
	
	class Models : public Resource
	{
	public:
		Models(const std::vector<std::shared_ptr<ModelData>>& a_ModelDatas);
		virtual ~Models() = default;
		const std::vector<std::shared_ptr<Model>>& GetModels() const { return m_Models; }
		
		static std::shared_ptr<Models> Load(const std::string& a_Path);
		static std::vector<Entity> LoadModelEntities(const std::string& a_Path);
		static std::vector<Entity> LoadModelEntitiesNew(const std::string& a_Path);
		static Entity LoadModelEntity(const std::string& a_Path);
		
	private:
		std::vector<std::shared_ptr<Model>> m_Models;
		
	};
}


