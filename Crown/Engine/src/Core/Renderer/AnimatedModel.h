#pragma once

#include <vector>
#include <glm/glm.hpp>

#ifdef CROWN_PS4

#endif

#ifdef CROWN_PC

#endif

namespace Crown
{
	const int maxBones = 128;
	const int maxBonesPerVertex = 4;
	
	struct AnimationVertexAttributes
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
		glm::ivec4 bones;
		glm::vec4 weights;
	};

	struct AnimationTexture
	{
		unsigned int m_texture;
		int width = -1;
		int height = -1;
	};

	class Joint;
	class Animation;
	struct AnimationDrawObject
	{
		std::vector<AnimationVertexAttributes> vertexBuffer;
		std::vector<unsigned int> indexBuffer;

		unsigned int animationVAO = -1;

		unsigned int posVBO = -1;
		unsigned int normalVBO = -1;
		unsigned int texCoordVBO = -1;

		unsigned int indexEBO = -1;

		AnimationTexture texture;

		Joint* rootJoint = nullptr;
		
		std::vector<glm::mat4> boneMatrices;
		std::vector<Animation> animations;
		size_t activeAnimationIndex = 0;
	};

	//Animation data: vector of key frames, each having vector of jointTransform with timestamp
	
	class Shader;
	
	class AnimatedModel
	{
	public:
		AnimatedModel();
		~AnimatedModel();

		void Update(float a_DeltaTime = 1.f / 120.f);
		void Draw(glm::mat4 a_MVP, glm::mat4 a_Model);

		void Load(std::string a_ModelRootDirectory, std::string a_ModelFileName);
		
		std::vector<std::string> GetAllAnimationNames(size_t a_PrimitiveID);
		bool SetAnimationByName(size_t a_PrimitiveID, const std::string a_AnimationName);

		Joint* GetJointByName(size_t a_PrimitiveID, const std::string a_JointName);
		
	private:
#ifdef CROWN_PC
		void LoadGLTFData(std::string a_ModelRootDirectory, std::string a_ModelFileName);
		void LoadGPUVertexAttributeData();

#endif
		
	private:
		
#ifdef CROWN_PC

		std::vector<AnimationDrawObject> m_DrawObjects;
		
		int m_MVPLoc = -1;
		int m_BonesLoc = -1;
		
		Shader* m_Shaders = nullptr;
		
#endif
	};
	
#ifdef CROWN_PC
	void LoadTexture(AnimationTexture& a_Texture, char const* a_FilePath);
#endif
}
