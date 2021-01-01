#pragma once

//#include <gltf.h>
#include <tiny_gltf.h>
#include <glm/glm.hpp>
#include <memory>

namespace Crown
{

	//Struct containing information about a buffer
	struct BufferData
	{
		BufferData() : accessor(nullptr), data(nullptr), dataSize(0), totalSize(0), emptySpace(0), numberOfElements(0) {}

		BufferData(::tinygltf::Accessor* a_Accessor, uint8_t const* a_Data, size_t a_DataSize, size_t a_EmptySpace, size_t a_NumberOfElements) :
			accessor(a_Accessor),
			data(a_Data),
			dataSize(a_DataSize),
			emptySpace(a_EmptySpace),
			numberOfElements(a_NumberOfElements)
		{
			//Calculate total size
			totalSize = static_cast<uint32_t>(a_Accessor->count) * a_DataSize;
		}

		//Pointer to accessor
		::tinygltf::Accessor* accessor;

		//Start of the data
		uint8_t const* data;

		//The size of each data element in bytes
		size_t dataSize;

		//The total size of all elements together in bytes
		size_t totalSize;

		//Empty space between objects. Measured from the end of one object to the start of the next
		size_t emptySpace;

		//The amount of elements
		size_t numberOfElements;

		bool HasData() const noexcept
		{
			return data != nullptr;
		}

		//Struct containing information about a buffer
		template<typename T>
		T const* GetElement(uint32_t index)
		{
			//Make sure there is data, and that this type is the correct type
			//Also check for out of bounds
			assert(HasData() && "Error! No data in this BufferData object.");
			assert(sizeof(T) == dataSize && "Error! Data type specified is not of the same length as the internal format.");
			assert(index < numberOfElements && "Error! Index out of bounds for BufferData.");

			//Calculate the position of the object required and return it
			return reinterpret_cast<T const*>(reinterpret_cast<size_t>(data) + (static_cast<size_t>(index) * static_cast<size_t>(emptySpace)) + (static_cast<size_t>(index) * static_cast<size_t>(dataSize)));
		};
	};

	enum InterpolationType
	{
		LINEAR,
		STEP,
		CUBICSPLINE
	};

	struct ChannelData
	{
		int samplerDataPositionIndex = -1;
		int samplerDataRotationIndex = -1;
		int samplerDataScaleIndex = -1;
		int jointIndex;
		bool hasJoint = false;
	};

	struct SamplerData
	{
		std::vector<float> inputs;
		std::vector<glm::vec4> vec4Outputs;
		std::vector<glm::vec3> vec3Outputs;
		InterpolationType InterpolationType = LINEAR;
	};

	struct JointData
	{
		std::string name;
		glm::vec3 position;
		glm::vec4 rotation;
		glm::vec3 scale;
		glm::mat4 matrix;

		std::vector<int> children;
		int jointIndex;
		int nodeIndex;
	};

	struct AnimationData
	{
		std::string name;
		std::vector<std::shared_ptr<ChannelData>> channelData;
		std::vector<std::shared_ptr<SamplerData>> samplerData;
	};

	struct AnimationModelData
	{
		std::vector<std::shared_ptr<JointData>> jointData;
		std::vector<std::shared_ptr<AnimationData>> animationData;
	};

	//Struct containing information about a Material
	struct MaterialData
	{
		//Base color of materialData
		glm::vec4 baseColor;
		//Metalic factor
		double metalFactor;
		//Roughness factor
		double roughnessFactor;

		//Properties
		::tinygltf::Material properties;

		///All possible textures as images
		::tinygltf::Image normalTextureImage;
		::tinygltf::Image occlusionTexturImage;
		::tinygltf::Image baseColorTextureImage;
		::tinygltf::Image metallicRoughnessTextureImage;
		::tinygltf::Image emissiveTextureImage;
	};

	//Temporary Vertext struct
	struct Vertex
	{
		Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 tex) : pos(pos), normal(normal), tex(tex) {}
		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 tex;

	};

	//Struct containing information about a primitive
	struct DrawObjectData
	{
		//Index of this node
		std::uint16_t nodeIndex;

		//Parsed vertices from position, normal and texCoord buffers
		std::vector<Vertex> vertices;

		//Indices
		std::vector<unsigned int> indices;

		std::shared_ptr<MaterialData> material;

		///For animation only
		std::vector<glm::i16vec4> joints;
		std::vector<glm::vec4> weights;

		//All buffers
		BufferData indexBuffer;
		BufferData positionBuffer;
		BufferData normalBuffer;
		BufferData texCoordBuffer;
		BufferData jointBuffer;
		BufferData weightBuffer;
	};

	struct ModelData
	{
		//Name of model
		std::string name;
		//Node of model
		::tinygltf::Node node;
		//Properties of mesh in model
		::tinygltf::Mesh properties;
		//Vector of primitives of the model
		std::vector<std::shared_ptr<DrawObjectData>> primitives;
		//Vector of all children models of this model
		std::vector<std::shared_ptr<ModelData>> children;
		//Material containing all textures of this model
		//std::shared_ptr<MaterialData> materialData;
		//
		//Animations
		std::shared_ptr<AnimationModelData> animationModelData;
	};

	struct CameraData
	{
		//Properties of CameraComponentSystem
		::tinygltf::Camera m_Properties;
		//Node of this camera
		::tinygltf::Node m_NodeTransform;
	};

	struct LightData
	{
		//Properties of lights
		::tinygltf::Light m_Properties;
		//Node of this light
		::tinygltf::Node m_NodeTransform;
	};

	class GLTFParser
	{
	public:
		//Loads a gltf file, returns a nested hierarchy of ModelData objects
		static std::vector<std::shared_ptr<ModelData>> LoadGLTFMeshesFromPath(const char* a_Path);

		//Loads all gltf model data of a single object based on a node in a tinygltf::Model
		static std::shared_ptr<ModelData> LoadGLTFMeshFromNode(::tinygltf::Model a_Document, ::tinygltf::Node a_Node);

		//Loads animation data from gltf path
		static std::shared_ptr<ModelData> LoadGLTFAnimation(const char* a_Path);

		//Loads Material data from a gltf primtive
		static std::shared_ptr<MaterialData> LoadGLTFMaterialDataFromPrimtive(::tinygltf::Model a_Document, ::tinygltf::Primitive a_Primitive);

		//Loads all gltf camera data of a single object based on a node in a tinygltf::Model
		static std::vector<std::shared_ptr<CameraData>> LoadGLTFCamerasFromPath(const char* a_Path);

		//Loads all gltf camera data of a single object based on a node in a tinygltf::Model
		static std::vector<std::shared_ptr<LightData>> LoadGLTFLightsFromPath(const char* a_Path);

		//Gets buffer data from model based on an index
		static BufferData GetBufferData(::tinygltf::Model& a_Model, std::uint32_t a_AttributeIndex);

		//Gets the full data size of a single element in the accessor
		static size_t DataTypeSize(::tinygltf::Accessor a_Accessor);

		static bool HasNodeExtras(::tinygltf::Node& a_Node);
		static std::vector<std::pair<std::string, std::string>> GetTagsFromNode(::tinygltf::Node& a_Node);
	};
}


