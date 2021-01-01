#include "CrownPCH.h"

#include "../src/Core/Renderer/AnimatedModel.h"

#include "Core/Renderer/GLTFLoading/GLTFParser.h"
#include "Platform/PC/Renderer/Shader.h"
#include "Core/Renderer/Joint.h"

#include "Core/Renderer/Animation.h"

#include "stb_image.h"

namespace Crown
{
	AnimatedModel::AnimatedModel()
	{
		m_Shaders = new Shader(
			"../Engine/assets/shaders/GLSL/AnimatedVertexShader.vert", 
			"../Engine/assets/shaders/GLSL/AnimatedFragmentShader.frag"
		);
	}

	AnimatedModel::~AnimatedModel()
	{
		delete m_Shaders;

		for(auto& drawObject : m_DrawObjects)
		{
			delete drawObject.rootJoint;
		}
	}

	void AnimatedModel::Load(std::string a_ModelRootDirectory, std::string a_ModelFileName)
	{
		LoadGLTFData(a_ModelRootDirectory, a_ModelFileName);
		LoadGPUVertexAttributeData();
	}

	std::vector<std::string> AnimatedModel::GetAllAnimationNames(size_t a_PrimitiveID)
	{
		std::vector<std::string> m_AnimationNames;
		if (a_PrimitiveID > m_DrawObjects.size()) { return m_AnimationNames; }

		for(auto& animation : m_DrawObjects[a_PrimitiveID].animations)
		{
			m_AnimationNames.push_back(animation.GetName());
		}

		return m_AnimationNames;
	}

	bool AnimatedModel::SetAnimationByName(size_t a_PrimitiveID, const std::string a_AnimationName)
	{
		if (a_PrimitiveID > m_DrawObjects.size()) { return false; }

		std::vector<Animation>& animations = m_DrawObjects[a_PrimitiveID].animations;
		
		for(int i = 0; i < animations.size(); i++)
		{
			if (animations[i].GetName() == a_AnimationName)
			{
				m_DrawObjects[a_PrimitiveID].activeAnimationIndex = i;
				return true;
			}
		}

		return false;
	}

	Joint* AnimatedModel::GetJointByName(size_t a_PrimitiveID, const std::string a_JointName)
	{
		if (a_PrimitiveID > m_DrawObjects.size()) { return nullptr; }
		
		return m_DrawObjects[a_PrimitiveID].rootJoint->FindJointByName(a_JointName);
	}

	void Crown::AnimatedModel::LoadGLTFData(std::string a_ModelRootDirectory, std::string a_ModelFileName)
	{
		std::string modelFilePath = a_ModelRootDirectory + a_ModelFileName;
		std::shared_ptr<ModelData> testModelData = GLTFParser::LoadGLTFAnimation(modelFilePath.c_str());
		
		m_DrawObjects.resize(testModelData->primitives.size());

		for(int drawObjectIndex = 0; drawObjectIndex < m_DrawObjects.size(); ++drawObjectIndex)
		{
			AnimationDrawObject& drawObject = m_DrawObjects[drawObjectIndex];
			const DrawObjectData& drawObjectData = *(testModelData->primitives[drawObjectIndex]);
			
			drawObject.vertexBuffer.resize(drawObjectData.vertices.size());

			for (int i = 0; i < drawObject.vertexBuffer.size(); ++i)
			{
				drawObject.vertexBuffer[i].position = drawObjectData.vertices[i].pos;
				drawObject.vertexBuffer[i].normal = drawObjectData.vertices[i].normal;
				drawObject.vertexBuffer[i].texCoord = drawObjectData.vertices[i].tex;

				drawObject.vertexBuffer[i].bones = drawObjectData.joints[i];
				drawObject.vertexBuffer[i].weights = drawObjectData.weights[i];
			}
			
			drawObject.indexBuffer.resize(drawObject.vertexBuffer.size());

			for (int i = 0; i < drawObject.indexBuffer.size(); ++i)
			{
				drawObject.indexBuffer[i] = i;
			}

			std::string imageUri = drawObjectData.material->baseColorTextureImage.uri;

			if(!imageUri.empty())
			{
				std::string textureFilePath = a_ModelRootDirectory + imageUri;
				LoadTexture(drawObject.texture, textureFilePath.c_str());
			}

			if(drawObjectIndex == 0)
			{
				delete drawObject.rootJoint;
				drawObject.rootJoint = new Joint(testModelData->animationModelData->jointData, 0, nullptr);
				drawObject.boneMatrices.resize(testModelData->animationModelData->jointData.size());

				for(auto& animationData : testModelData->animationModelData->animationData)
				{
					drawObject.animations.emplace_back(*animationData);
				}
			}
		}
	}

	void AnimatedModel::LoadGPUVertexAttributeData()
	{
		for (auto& drawObject : m_DrawObjects)
		{
			glGenVertexArrays(1, &drawObject.animationVAO);
			glBindVertexArray(drawObject.animationVAO);

			const int stride = 8 * sizeof(float) + maxBonesPerVertex * sizeof(int) + maxBonesPerVertex * sizeof(float);

			//vertex buffer data
			glGenBuffers(1, &drawObject.posVBO);
			glBindBuffer(GL_ARRAY_BUFFER, drawObject.posVBO);
			glBufferData(
				GL_ARRAY_BUFFER,
				stride * drawObject.vertexBuffer.size(),
				drawObject.vertexBuffer.data(),
				GL_STATIC_DRAW);

			//vertex position data
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, static_cast<void*>(nullptr));
			glEnableVertexAttribArray(0);
			//normal coordinates data
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			//texture coordinates data
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
			//bones coordinates data
			glVertexAttribIPointer(3, maxBonesPerVertex, GL_INT, stride, reinterpret_cast<void*>(8 * sizeof(float)));
			glEnableVertexAttribArray(3);
			//weights coordinates data
			glVertexAttribPointer(4, maxBonesPerVertex, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(8 * sizeof(float) + maxBonesPerVertex * sizeof(int)));
			glEnableVertexAttribArray(4);

			glGenBuffers(1, &drawObject.indexEBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, drawObject.indexEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * drawObject.indexBuffer.size(), drawObject.indexBuffer.data(), GL_STATIC_DRAW);

			m_Shaders->Use();
			m_MVPLoc = glGetUniformLocation(m_Shaders->ID, "gMVP");
			m_BonesLoc = glGetUniformLocation(m_Shaders->ID, "gBones");

			m_Shaders->SetInt("texture0", 0);

			glBindVertexArray(0);
		}
	}

	void LoadTexture(AnimationTexture& a_Texture, char const* a_FilePath)
	{
		glGenTextures(1, &a_Texture.m_texture);
		glBindTexture(GL_TEXTURE_2D, a_Texture.m_texture);
		//Wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//Filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_set_flip_vertically_on_load(false);

		int ncrChannels = 0;

		//Texture 0 
		unsigned char* data = stbi_load(a_FilePath, &a_Texture.width, &a_Texture.height, &ncrChannels, 0);
		if (data)
		{
			switch (ncrChannels)
			{
			case 3:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, a_Texture.width, a_Texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
			case 4:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, a_Texture.width, a_Texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
				break;
			default:
				assert(false);
				break;
			}
		}
		else
		{
			LOG("FAILED TO LOAD TEXTURE");
			assert(false);
		}
		stbi_image_free(data);
	}

	void Crown::AnimatedModel::Update(float a_DeltaTime)
	{
		for (auto& drawObject : m_DrawObjects)
		{
			if (drawObject.boneMatrices.size())
			{
				drawObject.animations[drawObject.activeAnimationIndex].Update(*drawObject.rootJoint, a_DeltaTime);
			}
		}
	}

	void Crown::AnimatedModel::Draw(glm::mat4 a_MVP, glm::mat4 a_Model)
	{
		m_Shaders->Use();

		m_Shaders->setMat4("gMVP", a_MVP);

		glActiveTexture(GL_TEXTURE0);
		
		for(auto& drawObject : m_DrawObjects)
		{		
			if(drawObject.texture.width > 0)
			{
				glBindTexture(GL_TEXTURE_2D, drawObject.texture.m_texture);
			}
			else
			{
				glBindTexture(GL_TEXTURE_2D, 0);
			}

			if(drawObject.boneMatrices.size())
			{
				drawObject.rootJoint->UpdateBoneMatrices(drawObject.boneMatrices, a_Model, nullptr);
				
				glUniformMatrix4fv(
					m_BonesLoc, 
					static_cast<GLsizei>(drawObject.boneMatrices.size()),
					GL_FALSE, 
					&drawObject.boneMatrices.front()[0][0]
				);
			}
			else
			{
				//TODO: unbind matrices
			}
			
			glBindVertexArray(drawObject.animationVAO);

			glDrawElements(GL_TRIANGLES, static_cast<int>(drawObject.indexBuffer.size()), GL_UNSIGNED_INT, 0);
		}
	}
}