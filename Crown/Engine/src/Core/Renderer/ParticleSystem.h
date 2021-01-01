#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace Crown
{
	struct ParticleAttribData
	{
		glm::vec3 position{};
		glm::vec4 color{};
		glm::vec2 scale{};
		float rotation{};
		glm::vec2 texCoordOffset{};
	};

	struct ParticleSystemParameters
	{
		//The lifetime (in seconds) of a particle
		float minLife = 1.5f;
		float maxLife = 3.0f;
		
		//The spawn location of a particle
		glm::vec3 minPosition = glm::vec3(-0.1f, -0.1f, -0.1f);
		glm::vec3 maxPosition = glm::vec3(0.1f, 0.1f, 0.1f);

		//The (tint) color applied to a particle
		glm::vec4 minColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//The direction and speed at which a particle travels
		glm::vec3 minVelocity = glm::normalize(glm::vec3(-0.3f, 0.0f, -1.0f)) * 5.f;
		glm::vec3 maxVelocity = glm::normalize(glm::vec3(0.3f, 0.5f, -0.5f)) * 3.f;

		//Specifies an acceleration to be continuously applied to the velocity of every particle (in a second)
		glm::vec3 acceleration = glm::vec3(0.0f, 0.2f, -0.4f);

		//The scale of the particle
		float minSize = 0.25f;
		float maxSize = 1.75f;

		//The rate (in seconds) at which particles spawn
		float spawnRate = 0.05f;

		//Whether the color alpha value decreases based on particle remaining life
		bool doesFadeOut = true;
		//Used to affect alpha color value of particle
		float opacityMultiplier = 1.f;

		//File path to the texture to be applied to the particles
		std::string textureFilePath = "assets/textures/smoke_texture_atlas_2.png"  /*"assets/textures/mario_on_PS4.jpg"*/;

		//The two below variables are in relation to the TextureAtlas which is used for animations
		//They define the number of columns and rows present in the texture atlas
		//If a texture atlas is not used, then set both values to 1
		int xTexDivisions = 7;
		int yTexDivisions = 7;
		
		//float minLife = 2.0f;
		//float maxLife = 4.0f;
		//
		//glm::vec3 minPosition = glm::vec3(-0.1f, -0.1f, -0.1f);
		//glm::vec3 maxPosition = glm::vec3(0.1f, 0.1f, 0.1f);

		//glm::vec4 minColor = glm::vec4(1.0f, 0.65f, 0.65f, 1.0f);
		//glm::vec4 maxColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//glm::vec3 minVelocity = glm::vec3(-0.75f, 0.0f, -0.75f);
		//glm::vec3 maxVelocity = glm::vec3(0.75f, 1.0f, 0.75f);

		//glm::vec3 acceleration = glm::vec3(0.1f, 0.8f, 0.0f);

		//float minSize = 0.5f;
		//float maxSize = 1.5f;

		////whether the color alpha value decreases based on particle remaining life
		//bool doesFadeOut = true;
		////used to affect alpha color value of particle
		//float opacityMultiplier = 0.5f;

		//std::string textureFilePath = "assets/textures/fire_texture_atlas.png";

		//int xTexDivisions = 8;
		//int yTexDivisions = 6;

		//float spawnRate = 0.035f;
		
	};

	struct ParticleData
	{
		ParticleAttribData attribData{};
		float maxLife{};
		float currentLife{};
		glm::vec3 velocity{};
		glm::vec3 acceleration{};

		ParticleData()
			: currentLife(0.0f), velocity(0.0f) { }

	};

	class ParticleRenderData;

	//TODO: in future, this should be made into a component (and be associated a transform)
	class ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();

		const ParticleSystemParameters& GetParams() const { return m_Params; }
		void SetParams(ParticleSystemParameters& a_Params) { m_Params = a_Params; }
		
		void Update(float a_DeltaTime);
		ParticleRenderData& GetParticleRenderData();

		int GetLiveParticleCount() const { return m_LiveParticleCount; }
		const std::vector<ParticleAttribData>& GetInstanceBuffer() const { return m_InstanceBuffer; }

		void SortParticlesBackToFront(const glm::mat4& a_MVP);
	private:
		size_t FindAvailableParticle();

		void TrySpawnParticles(float a_DeltaTime);
		void SpawnParticle(size_t ParticleIndex);

		void UpdateParticles(float a_DeltaTime);
		void UpdateParticle(ParticleData& p, float a_DeltaTime);
	public:
		static const float ms_BillboardVertices[30];
		static const size_t ms_MaxParticles = 128;
	private:
		ParticleRenderData* m_ParticleRenderData = nullptr;

		ParticleSystemParameters m_Params{};

		std::vector<ParticleAttribData> m_InstanceBuffer{};
		int m_LiveParticleCount = 0;

		std::vector<ParticleData> m_Particles{};

		float m_ParticleSpawnCooldown = 0.f;
		size_t m_LastSpawnedParticleIndex = 0;
	};

	void CalculateTexCoordOffset(glm::vec2& offset, int texXDivisions, int texYDivisions, float progression);
	
	float RandomFloat(float a_Min, float a_Max);

	template<typename T>
	void RandomizeVec(T& a_result, const T& a_Min, const T& a_Max, int size)
	{
		for (int i = 0; i < size; ++i)
		{
			a_result[i] = RandomFloat(a_Min[i], a_Max[i]);
		}
	}
}