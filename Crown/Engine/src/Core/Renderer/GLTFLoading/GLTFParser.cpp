#include "CrownPCH.h"

#include "GLTFParser.h"

std::vector<std::shared_ptr<Crown::ModelData>> Crown::GLTFParser::LoadGLTFMeshesFromPath(const char* a_Path)
{
	CROWN_PROFILE_FUNCTION();

	ASSERT_ENGINE(a_Path != NULL && a_Path[0] != '\0', "File that one is trying to parse GLTF Meshes with is empty");

	//Hierarchy to return
	std::vector<std::shared_ptr<ModelData>> hierarchy;

	//Load in whole document
	tinygltf::TinyGLTF loader;
	tinygltf::Model doc;
	std::string err;
	std::string warn;

	//Detect if binary or ascii gltf file & handle loading
	const std::string binaryExt = ".glb";
	const std::string asciiExt = ".gltf";
	const std::string strPath = static_cast<std::string>(a_Path);
	if (strPath.substr(strPath.length() - 4) == binaryExt)
	{
		loader.LoadBinaryFromFile(&doc, &err, &warn, a_Path);
	}
	else if (strPath.substr(strPath.length() - 5) == asciiExt)
	{
		loader.LoadASCIIFromFile(&doc, &err, &warn, a_Path);
	}
	else
	{
		LOG("NOT AN ASCII OR BINARY GLTF FILE");
		err = "given file is not a gltf file";
	}


	//Assert when we receive an error or warning
	ASSERT_ENGINE(err.empty(), err);
	ASSERT_ENGINE(warn.empty(), warn);

	//Get scene in order to search for the correct nodes in the document
	const tinygltf::Scene& scene = doc.scenes[0];

	for (size_t i = 0; i < scene.nodes.size(); ++i)
	{
		std::shared_ptr<ModelData> modelData = GLTFParser::LoadGLTFMeshFromNode(doc, doc.nodes[scene.nodes[i]]);
		modelData->node = doc.nodes[scene.nodes[i]];

		if (modelData->node.translation.empty())
		{
			for (size_t j = 0; j < 3; j++)
			{
				modelData->node.translation.push_back(0.0f);
			}
		}

		if (modelData->node.rotation.empty())
		{
			for (size_t j = 0; j < 3; j++)
			{
				modelData->node.rotation.push_back(0.0f);
			}
		}

		if (modelData->node.scale.empty())
		{
			for (size_t j = 0; j < 3; j++)
			{
				modelData->node.scale.push_back(0.0f);
			}
		}

		hierarchy.push_back(modelData);

		for (size_t j = 0; j < modelData->children.size(); j++)
		{
			if (modelData->children[j]->node.translation.empty())
			{
				for (size_t j = 0; j < 3; j++)
				{
					modelData->node.translation.push_back(0.0f);
				}
			}

			if (modelData->children[j]->node.rotation.empty())
			{
				for (size_t j = 0; j < 3; j++)
				{
					modelData->node.rotation.push_back(0.0f);
				}
			}

			if (modelData->children[j]->node.scale.empty())
			{
				for (size_t j = 0; j < 3; j++)
				{
					modelData->node.scale.push_back(0.0f);
				}
			}

			hierarchy.push_back(modelData->children[i]);
		}
	}

	return hierarchy;
}

std::shared_ptr<Crown::ModelData> Crown::GLTFParser::LoadGLTFMeshFromNode(tinygltf::Model a_Document, tinygltf::Node a_Node)
{
	CROWN_PROFILE_FUNCTION();

	const tinygltf::Node& node = a_Node;
	bool loaded = false;
	std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
	modelData->name = node.name;

	if (node.mesh >= 0)
	{
		loaded = true;
		//Creation and assigning of mesh object
		modelData->properties = a_Document.meshes[node.mesh];

		for (int primitiveID = 0; primitiveID < modelData->properties.primitives.size(); primitiveID++)
		{
			//Local ref to primitive
			auto primitiveData = modelData->properties.primitives[primitiveID];
			std::shared_ptr<DrawObjectData> primitive = std::make_shared<DrawObjectData>();

			///Set index buffer
			if (primitiveData.indices >= 0)
				primitive->indexBuffer = GetBufferData(a_Document, primitiveData.indices);

			bool loadPosition = false;
			bool loadNormal = false;
			bool loadTexture = false;
			for (auto const& attrib : primitiveData.attributes)
			{
				//Get the buffer of positions, normals and texcoords
				if (attrib.first == "POSITION")
				{
					loadPosition = true;
					primitive->positionBuffer = GetBufferData(a_Document, attrib.second);
				}
				else if (attrib.first == "NORMAL")
				{
					loadNormal = true;
					primitive->normalBuffer = GetBufferData(a_Document, attrib.second);;
				}
				else if (attrib.first == "TEXCOORD_0")
				{
					loadTexture = true;
					primitive->texCoordBuffer = GetBufferData(a_Document, attrib.second);;
				}
				else if (attrib.first == "JOINTS_0")
				{
					primitive->jointBuffer = GetBufferData(a_Document, attrib.second);
				}
				else if (attrib.first == "WEIGHTS_0")
				{
					primitive->weightBuffer = GetBufferData(a_Document, attrib.second);
				}
			}

			//Build all the vertices.
			for (std::uint32_t index = 0; index < primitive->positionBuffer.numberOfElements; ++index)
			{
				//Calculate the offsets into the buffers where the current stuff resides.
				const glm::vec3 position = *primitive->positionBuffer.GetElement<glm::vec3>(index);
				glm::vec3 normal = glm::vec3();

				if (loadNormal)
					normal = *primitive->normalBuffer.GetElement<glm::vec3>(index);

				glm::vec2 texCoord = glm::vec2();

				if (loadTexture)
					texCoord = *primitive->texCoordBuffer.GetElement<glm::vec2>(index);

				//Put the new vertex in the vertices vector.
				primitive->vertices.emplace_back(Vertex(position, normal, texCoord));

			}

			//Joints
			for (std::uint32_t index = 0; index < primitive->jointBuffer.numberOfElements; ++index)
			{
				auto joint = *primitive->jointBuffer.GetElement<glm::i16vec4>(index);
				primitive->joints.emplace_back(joint);
			}

			//Weights
			for (std::uint32_t index = 0; index < primitive->weightBuffer.numberOfElements; ++index)
			{
				auto weight = *primitive->weightBuffer.GetElement<glm::vec4>(index);
				primitive->weights.emplace_back(weight);
			}

			if (primitiveData.indices == -1)
			{
				for (std::uint32_t index = 0; index < primitive->positionBuffer.numberOfElements; ++index)
				{
					primitive->indices.emplace_back(0);
				}
			}
			else
			{
				//Insert all the indices in the index buffer.
				primitive->indices.reserve(primitive->indexBuffer.numberOfElements);
				for (std::uint32_t i = 0; i < primitive->indexBuffer.numberOfElements; ++i)
				{
					auto index = *primitive->indexBuffer.GetElement<uint16_t>(i);
					primitive->indices.emplace_back(index);
				}
			}

			//Load materialData for primitive
			primitive->material = LoadGLTFMaterialDataFromPrimtive(a_Document, primitiveData);

			modelData->primitives.push_back(primitive);
		}
	}

	//Assign children
	for (size_t i = 0; i < node.children.size(); i++)
	{
		if (a_Document.nodes[node.children[i]].mesh >= 0)
		{
			std::shared_ptr<ModelData> data = LoadGLTFMeshFromNode(a_Document, a_Document.nodes[node.children[i]]);
			modelData->children.push_back(data);
		}
	}

	if (loaded)
	{
		return modelData;
	}
	else
	{
		return nullptr;
	}

	//CROWN_PROFILE_FUNCTION();

	//const tinygltf::Node& node = a_Node;

	//std::shared_ptr<ModelData> modelData = std::make_shared<ModelData>();
	//modelData->name = node.name;
	//
	//if (node.mesh >= 0)
	//{
	//	//Creation and assigning of mesh object
	//	modelData->properties = a_Document.meshes[node.mesh];

	//	for (int primitiveID = 0; primitiveID < modelData->properties.primitives.size(); primitiveID++)
	//	{
	//		//Local ref to primitive
	//		auto primitiveData = modelData->properties.primitives[primitiveID];
	//		std::shared_ptr<DrawObjectData> primitive = std::make_shared<DrawObjectData>();
	//		
	//		///Set index buffer
	//		primitive->indexBuffer = GetBufferData(a_Document, primitiveData.indices);

	//		bool loadPosition = false;
	//		bool loadNormal = false;
	//		bool loadTexture = false;
	//		for (auto const& attrib : primitiveData.attributes)
	//		{
	//			//Get the buffer of positions, normals and texcoords
	//			if (attrib.first == "POSITION")
	//			{
	//				loadPosition = true;
	//				primitive->positionBuffer = GetBufferData(a_Document, attrib.second);
	//			}
	//			else if (attrib.first == "NORMAL")
	//			{
	//				loadNormal = true;
	//				primitive->normalBuffer = GetBufferData(a_Document, attrib.second);;
	//			}
	//			else if (attrib.first == "TEXCOORD_0")
	//			{
	//				loadTexture = true;
	//				primitive->texCoordBuffer = GetBufferData(a_Document, attrib.second);;
	//			}
	//		}

	//		//Build all the vertices.
	//		for (std::uint32_t index = 0; index < primitive->positionBuffer.numberOfElements; ++index)
	//		{
	//			//Calculate the offsets into the buffers where the current stuff resides.
	//			const glm::vec3 position = *primitive->positionBuffer.GetElement<glm::vec3>(index);
	//			const glm::vec3 normal = *primitive->normalBuffer.GetElement<glm::vec3>(index);
	//			glm::vec2 texCoord = glm::vec2();

	//			if(loadTexture)
	//				texCoord = *primitive->texCoordBuffer.GetElement<glm::vec2>(index);

	//			//Put the new vertex in the vertices vector.
	//			primitive->vertices.emplace_back(Vertex(position, normal, texCoord));

	//		}

	//		//Insert all the indices in the index buffer.
	//		primitive->indices.reserve(primitive->indexBuffer.numberOfElements);
	//		for (std::uint32_t i = 0; i < primitive->indexBuffer.numberOfElements; ++i)
	//		{
	//			auto index = *primitive->indexBuffer.GetElement<uint16_t>(i);
	//			primitive->indices.emplace_back(index);
	//		}

	//		//Load materialData for primitive
	//		primitive->materialData = LoadGLTFMaterialDataFromPrimtive(a_Document, primitiveData);
	//		
	//		modelData->primitives.push_back(primitive);
	//	}
	//}

	////Assign children
	//for (size_t i = 0; i < node.children.size(); i++)
	//{
	//	std::shared_ptr<ModelData> data = LoadGLTFMeshFromNode(a_Document, a_Document.nodes[node.children[i]]);
	//	modelData->children.push_back(data);
	//}

	//return modelData;
}

std::shared_ptr<Crown::ModelData> Crown::GLTFParser::LoadGLTFAnimation(const char* a_Path)
{
	//Load in whole document
	tinygltf::TinyGLTF loader;
	tinygltf::Model doc;
	std::string err;
	std::string warn;
	loader.LoadASCIIFromFile(&doc, &err, &warn, a_Path);

	//Get scene in order to search for the correct nodes in the document
	const tinygltf::Scene& scene = doc.scenes[0];
	//If no animation then return
	if (doc.animations.size() == 0) return nullptr;

	//Find the actual skin node and skin
	tinygltf::Node skinNode;
	tinygltf::Skin skin;
	for (auto node : doc.nodes)
	{
		if (node.skin >= 0)
		{
			skinNode = node;
			skin = doc.skins[skinNode.skin];
		}
	}

	std::shared_ptr<Crown::AnimationModelData> animModelData = std::make_shared<AnimationModelData>();

	for (int animationIndex = 0; animationIndex < doc.animations.size(); ++animationIndex)
	{
		tinygltf::Animation animation = doc.animations[animationIndex];
		std::shared_ptr<AnimationData> animData = std::make_shared<AnimationData>();
		animModelData->animationData.push_back(animData);
		animData->name = animation.name;

#pragma region channels and samplers
		for (int i = 0; i < animation.channels.size(); ++i)
		{
			std::shared_ptr<ChannelData> cd = std::make_shared<ChannelData>();

			cd->samplerDataPositionIndex = -1;
			cd->samplerDataRotationIndex = -1;
			cd->samplerDataScaleIndex = -1;

			if (animation.channels[i].target_path == "translation")
			{
				cd->samplerDataPositionIndex = animation.channels[i].sampler;
			}
			else if (animation.channels[i].target_path == "rotation")
			{
				cd->samplerDataRotationIndex = animation.channels[i].sampler;
			}
			else if (animation.channels[i].target_path == "scale")
			{
				cd->samplerDataScaleIndex = animation.channels[i].sampler;
			}

			for (int jointIndex = 0; jointIndex < skin.joints.size(); ++jointIndex)
			{
				if (skin.joints[jointIndex] == animation.channels[i].target_node)
				{
					cd->jointIndex = jointIndex;
					break;
				}
			}

			animData->channelData.push_back(cd);
		}
		int outputIndex = 0;
		for (int i = 0; i < animation.samplers.size(); i++)
		{
			std::shared_ptr<SamplerData> sd = std::make_shared<SamplerData>();
			BufferData input = GetBufferData(doc, animation.samplers[i].input);
			BufferData output = GetBufferData(doc, animation.samplers[i].output);

			for (int j = 0; j < input.numberOfElements; ++j)
			{
				sd->inputs.push_back(*input.GetElement<float>(j));
			}

			for (int j = 0; j < output.numberOfElements; ++j)
			{
				if (output.dataSize == 16)
				{
					glm::vec4 data = *output.GetElement<glm::vec4>(j);
					sd->vec4Outputs.push_back(data);
				}
				else if (output.dataSize == 12)
				{
					glm::vec3 data = *output.GetElement<glm::vec3>(j);
					sd->vec3Outputs.push_back(data);
				}
			}
			outputIndex++;
			if (outputIndex == 3) outputIndex = 0;
			animData->samplerData.push_back(sd);
		}
#pragma endregion	
	}

#pragma region skin and joints
	for (int i = 0; i < skin.joints.size(); ++i)
	{
		std::shared_ptr<JointData> jointData = std::make_shared<JointData>();
		jointData->jointIndex = i;
		jointData->nodeIndex = skin.joints[i];
		jointData->name = doc.nodes[jointData->nodeIndex].name;

		if (doc.nodes[skin.joints[i]].translation.empty())
		{
			jointData->position = glm::vec3(0, 0, 0);
		}
		else
		{
			jointData->position = glm::vec3(
				doc.nodes[skin.joints[i]].translation[0],
				doc.nodes[skin.joints[i]].translation[1],
				doc.nodes[skin.joints[i]].translation[2]);
		}

		if (doc.nodes[skin.joints[i]].rotation.empty())
		{
			jointData->rotation = glm::vec4(0, 0, 0, 0);
		}
		else
		{
			jointData->rotation = glm::vec4(
				doc.nodes[skin.joints[i]].rotation[0],
				doc.nodes[skin.joints[i]].rotation[1],
				doc.nodes[skin.joints[i]].rotation[2],
				doc.nodes[skin.joints[i]].rotation[3]);
		}

		if (doc.nodes[skin.joints[i]].scale.empty())
		{
			jointData->scale = glm::vec3(1, 1, 1);
		}
		else
		{
			jointData->scale = glm::vec3(
				doc.nodes[skin.joints[i]].scale[0],
				doc.nodes[skin.joints[i]].scale[1],
				doc.nodes[skin.joints[i]].scale[2]);
		}

		animModelData->jointData.push_back(jointData);
	}
	//Assigning child joints
	for (int i = 0; i < animModelData->jointData.size(); i++)
	{
		tinygltf::Node node = doc.nodes[animModelData->jointData[i]->nodeIndex];

		for (auto child : node.children)
		{
			for (int x = 0; x < animModelData->jointData.size(); x++)
			{
				if (child == animModelData->jointData[x]->nodeIndex)
				{
					animModelData->jointData[i]->children.push_back(animModelData->jointData[x]->jointIndex);
				}
			}
		}
	}

	//Inverse bind matrix
	BufferData matrix = GetBufferData(doc, skin.inverseBindMatrices);

	//Assign all matrix
	for (int jointIndex = 0; jointIndex < animModelData->jointData.size(); ++jointIndex)
	{
		animModelData->jointData[jointIndex]->matrix = *matrix.GetElement<glm::mat4>(jointIndex);
	}
#pragma endregion 

	for (int animationIndex = 0; animationIndex < animModelData->animationData.size(); ++animationIndex)
	{
		for (int i = 0; i < animModelData->jointData.size(); i++)
		{
			tinygltf::Node node = doc.nodes[animModelData->jointData[i]->nodeIndex];

			for (auto child : node.children)
			{
				for (int x = 0; x < animModelData->animationData[animationIndex]->channelData.size(); x++)
				{
					if (!animModelData->animationData[animationIndex]->channelData[x]->hasJoint)
					{
						if (child == !animModelData->animationData[animationIndex]->channelData[x]->jointIndex)
						{
							animModelData->animationData[animationIndex]->channelData[x]->jointIndex = animModelData->jointData[i]->jointIndex;
							animModelData->animationData[animationIndex]->channelData[x]->jointIndex = true;
						}
					}
				}
			}
		}
	}

#pragma region Model
	std::shared_ptr<ModelData> modelData = LoadGLTFMeshFromNode(doc, skinNode);
	modelData->animationModelData = animModelData;
#pragma endregion 
	return modelData;
}

std::shared_ptr<Crown::MaterialData> Crown::GLTFParser::LoadGLTFMaterialDataFromPrimtive(::tinygltf::Model a_Document, ::tinygltf::Primitive a_Primitive)
{
	CROWN_PROFILE_FUNCTION();

	std::shared_ptr<MaterialData> drawObjectMaterial = std::make_shared<MaterialData>();

	if (a_Primitive.material >= 0)
	{
		tinygltf::Material& matProperties = a_Document.materials[a_Primitive.material];

		//Setting base color
		drawObjectMaterial->baseColor = glm::vec4(
			matProperties.pbrMetallicRoughness.baseColorFactor[0],
			matProperties.pbrMetallicRoughness.baseColorFactor[1],
			matProperties.pbrMetallicRoughness.baseColorFactor[2],
			matProperties.pbrMetallicRoughness.baseColorFactor[3]
		);
		//metal factor
		drawObjectMaterial->metalFactor = matProperties.pbrMetallicRoughness.metallicFactor;
		//Roughness factor
		drawObjectMaterial->roughnessFactor = matProperties.pbrMetallicRoughness.roughnessFactor;
		//Normal texture
		if (a_Document.materials[a_Primitive.material].normalTexture.index >= 0)
		{
			tinygltf::Texture& texture = a_Document.textures[a_Document.materials[a_Primitive.material].normalTexture.index];

			if (texture.source >= 0)
			{
				drawObjectMaterial->normalTextureImage = a_Document.images[texture.source];
			}
		}
		//Occlusion texture
		if (a_Document.materials[a_Primitive.material].occlusionTexture.index >= 0)
		{
			tinygltf::Texture& texture = a_Document.textures[a_Document.materials[a_Primitive.material].occlusionTexture.index];

			if (texture.source >= 0)
			{
				drawObjectMaterial->occlusionTexturImage = a_Document.images[texture.source];
			}
		}
		//Base color texture
		if (a_Document.materials[a_Primitive.material].pbrMetallicRoughness.baseColorTexture.index >= 0)
		{
			tinygltf::Texture& texture = a_Document.textures[a_Document.materials[a_Primitive.material].pbrMetallicRoughness.baseColorTexture.index];

			if (texture.source >= 0)
			{
				drawObjectMaterial->baseColorTextureImage = a_Document.images[texture.source];
			}
		}
		//Metallic roughness texture
		if (a_Document.materials[a_Primitive.material].pbrMetallicRoughness.metallicRoughnessTexture.index >= 0)
		{
			tinygltf::Texture& texture = a_Document.textures[a_Document.materials[a_Primitive.material].pbrMetallicRoughness.metallicRoughnessTexture.index];

			if (texture.source >= 0)
			{
				drawObjectMaterial->metallicRoughnessTextureImage = a_Document.images[texture.source];
			}
		}
		//Emissive texture
		if (a_Document.materials[a_Primitive.material].emissiveTexture.index >= 0)
		{
			tinygltf::Texture& texture = a_Document.textures[a_Document.materials[a_Primitive.material].emissiveTexture.index];

			if (texture.source >= 0)
			{
				drawObjectMaterial->emissiveTextureImage = a_Document.images[texture.source];
			}
		}
	}

	return drawObjectMaterial;
}

std::vector<std::shared_ptr<Crown::CameraData>> Crown::GLTFParser::LoadGLTFCamerasFromPath(const char* a_Path)
{
	CROWN_PROFILE_FUNCTION();

	//CameraComponentSystem vector to return
	std::vector<std::shared_ptr<CameraData>> cameras;

	//Load in whole document
	tinygltf::TinyGLTF loader;
	tinygltf::Model doc;
	std::string err;
	std::string warn;

	//Detect if binary or ascii gltf file & handle loading
	const std::string binaryExt = ".glb";
	const std::string asciiExt = ".gltf";
	const std::string strPath = static_cast<std::string>(a_Path);
	if (strPath.substr(strPath.length() - 4) == binaryExt)
	{
		loader.LoadBinaryFromFile(&doc, &err, &warn, a_Path);
	}
	else if (strPath.substr(strPath.length() - 5) == asciiExt)
	{
		loader.LoadASCIIFromFile(&doc, &err, &warn, a_Path);
	}
	else
	{
		LOG("NOT AN ASCII OR BINARY GLTF FILE");
		err = "given file is not a gltf file";
	}

	//Loop through each node to find a node with a camera property higher than -1
	for (size_t i = 0; i < doc.nodes.size(); ++i)
	{
		if (doc.nodes[i].camera >= 0)
		{
			///Create and assign node / camera data to the camera
			std::shared_ptr<CameraData> cameraData = std::make_shared<CameraData>();
			cameraData->m_NodeTransform = doc.nodes[i];
			cameraData->m_Properties = doc.cameras[cameraData->m_NodeTransform.camera];
			cameras.push_back(cameraData);
		}
	}

	return cameras;
}

std::vector<std::shared_ptr<Crown::LightData>> Crown::GLTFParser::LoadGLTFLightsFromPath(const char* a_Path)
{
	CROWN_PROFILE_FUNCTION();

	//Light vector to return
	std::vector<std::shared_ptr<LightData>> lights;

	//Load in whole document
	tinygltf::TinyGLTF loader;
	tinygltf::Model doc;
	std::string err;
	std::string warn;

	//Detect if binary or ascii gltf file & handle loading
	const std::string binaryExt = ".glb";
	const std::string asciiExt = ".gltf";
	const std::string strPath = static_cast<std::string>(a_Path);
	if (strPath.substr(strPath.length() - 4) == binaryExt)
	{
		loader.LoadBinaryFromFile(&doc, &err, &warn, a_Path);
	}
	else if (strPath.substr(strPath.length() - 5) == asciiExt)
	{
		loader.LoadASCIIFromFile(&doc, &err, &warn, a_Path);
	}
	else
	{
		LOG("NOT AN ASCII OR BINARY GLTF FILE");
		err = "given file is not a gltf file";
	}

	//Loop through each node to find a node with a light property higher than -1
	for (size_t i = 0; i < doc.nodes.size(); ++i)
	{
		if (doc.nodes[i].extensions.size() > 0)
		{
			//Find light extension
			auto it = doc.nodes[i].extensions.find("KHR_lights_punctual");
			if (it != doc.nodes[i].extensions.end())
			{
				if (it->second.IsObject())
				{
					///Find light attribute and make light
					double id = it->second.Get("light").GetNumberAsInt();
					std::shared_ptr<LightData> light = std::make_shared<LightData>();
					light->m_Properties = doc.lights[static_cast<int>(id)];;
					light->m_NodeTransform = doc.nodes[i];
					lights.push_back(light);
				}
			}
		}
	}

	return lights;
}

Crown::BufferData Crown::GLTFParser::GetBufferData(tinygltf::Model& a_Model, std::uint32_t a_AttributeIndex)
{
	CROWN_PROFILE_FUNCTION();

	//Accessor
	tinygltf::Accessor& accessor = a_Model.accessors[a_AttributeIndex];
	//Buffer view
	tinygltf::BufferView& bufferView = a_Model.bufferViews[accessor.bufferView];
	//Buffer
	tinygltf::Buffer& buffer = a_Model.buffers[bufferView.buffer];
	//Size of a single element in the accessor
	size_t dataTypeSize = DataTypeSize(accessor);
	//Calculate if there is any empty space
	size_t emptySpace = bufferView.byteStride == 0 ? 0 : bufferView.byteStride - dataTypeSize;

	//Return constructed object
	return BufferData(&accessor, &buffer.data[static_cast<uint64_t>(bufferView.byteOffset) + accessor.byteOffset], dataTypeSize, emptySpace, accessor.count);
}

size_t Crown::GLTFParser::DataTypeSize(tinygltf::Accessor a_Accessor)
{
	CROWN_PROFILE_FUNCTION();

	uint32_t elementSize = 0;

	//Get single element data type size
	switch (a_Accessor.componentType)
	{
	case TINYGLTF_COMPONENT_TYPE_BYTE:
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE:
		elementSize = 1;
		break;
	case TINYGLTF_COMPONENT_TYPE_SHORT:
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT:
		elementSize = 2;
		break;
	case TINYGLTF_COMPONENT_TYPE_FLOAT:
	case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT:
		elementSize = 4;
		break;
	}

	//Based on what the data is representing,
	//multiply
	switch (a_Accessor.type)
	{
	case TINYGLTF_TYPE_MAT2:
		return 4 * elementSize;
		break;
	case TINYGLTF_TYPE_MAT3:
		return 9 * elementSize;
		break;
	case TINYGLTF_TYPE_MAT4:
		return 16 * elementSize;
		break;
	case TINYGLTF_TYPE_SCALAR:
		return elementSize;
		break;
	case TINYGLTF_TYPE_VEC2:
		return 2 * elementSize;
		break;
	case TINYGLTF_TYPE_VEC3:
		return 3 * elementSize;
		break;
	case TINYGLTF_TYPE_VEC4:
		return 4 * elementSize;
		break;
	}

	return 0;
}

bool Crown::GLTFParser::HasNodeExtras(::tinygltf::Node& a_Node)
{
	if (!a_Node.extras.Keys().empty()) return true;
	return false;
}

std::vector<std::pair<std::string, std::string>> Crown::GLTFParser::GetTagsFromNode(::tinygltf::Node& a_Node)
{
	std::vector<std::pair<std::string, std::string>> extras;

	for (auto key : a_Node.extras.Keys())
	{
		if (a_Node.extras.Get(key).IsString())
		{
			std::string value = a_Node.extras.Get(key).GetNumberAsString();
			extras.push_back(std::pair<std::string, std::string>(key, value));
		}
	}

	return extras;
}
