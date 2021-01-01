#pragma once

#include "CrownPCH.h"

#include "Core/Renderer/ParticleRenderData.h"

#include <glm/glm.hpp>

namespace Crown
{
	struct ParticleTexture
	{
		unsigned int m_texture;
		int width = -1;
		int height = -1;

		int xDivisions = 1;
		int yDivisions = 1;
		glm::vec2 texAtlasCellSize = glm::vec2(1.0f, 1.0f);
	};
	
	class Shader;
	
	class PCParticleRenderData : public ParticleRenderData
	{
	public:
		PCParticleRenderData();
		virtual ~PCParticleRenderData();

		void Init(ParticleSystem* a_ParticleSystem) override;

		void LoadGPUData(const std::vector<ParticleAttribData>& a_InstanceData) const override;

		void Draw(const glm::mat4 a_MVP, const glm::mat4 a_CameraView) const;

	private:
		
		unsigned int m_ParticleVAO = -1;

		unsigned int m_BillboardVBO = -1;
		unsigned int m_PositionsVBO = -1;

		Shader* m_Shaders = nullptr;

		int m_ParticleMVPLoc = -1;
		int m_CameraRightLoc = -1;
		int m_CameraUpLoc = -1;
		int m_texAtlasCellSizeLoc = -1;

		ParticleTexture m_Texture;
	};

	void LoadTexture(ParticleTexture& a_Texture, char const* a_FilePath); 
}