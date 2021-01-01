#include "CrownPCH.h"

#include "PCParticleRenderData.h"

#include <cassert>
#include <algorithm>

#include "Core/Renderer/ParticleSystem.h"
#include "Shader.h"
#include "stb_image.h"

namespace Crown
{
	PCParticleRenderData::PCParticleRenderData()
	{
		
	}

	PCParticleRenderData::~PCParticleRenderData()
	{
		delete m_Shaders;
	}

	void PCParticleRenderData::Init(ParticleSystem* a_ParticleSystem)
	{
		assert(a_ParticleSystem);
		m_ParticleSystem = a_ParticleSystem;

		glGenVertexArrays(1, &m_ParticleVAO);
		glBindVertexArray(m_ParticleVAO);

		//quad buffer data
		glGenBuffers(1, &m_BillboardVBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_BillboardVBO);
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(ParticleSystem::ms_BillboardVertices),
			ParticleSystem::ms_BillboardVertices,
			GL_STATIC_DRAW);
		//vertex position data
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void*>(nullptr));
		glEnableVertexAttribArray(0);
		glVertexAttribDivisor(0, 0);
		//texture coordinates data
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glVertexAttribDivisor(1, 0);

		//add a call to loadGPU data here
		glGenBuffers(1, &m_PositionsVBO);
		LoadGPUData(m_ParticleSystem->GetInstanceBuffer());
		glBindVertexArray(m_ParticleVAO);
		
		//position
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), static_cast<void*>(nullptr));
		glEnableVertexAttribArray(2);
		glVertexAttribDivisor(2, 1);
		//color
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribDivisor(3, 1);
		//scale
		glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(7 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribDivisor(4, 1);
		//rotation
		glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(9 * sizeof(float)));
		glEnableVertexAttribArray(5);
		glVertexAttribDivisor(5, 1);
		//texCoordOffset
		glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, 12 * sizeof(float), reinterpret_cast<void*>(10 * sizeof(float)));
		glEnableVertexAttribArray(6);
		glVertexAttribDivisor(6, 1);

		LoadTexture(m_Texture, m_ParticleSystem->GetParams().textureFilePath.c_str());
		
		m_Shaders = new Shader("assets/shaders/GLSL/ParticleVertexShader.vert", "assets/shaders/GLSL/ParticleFragmentShader.frag");

		m_Shaders->Use();
		m_Shaders->SetInt("texture0", 0);
		
		m_ParticleMVPLoc = glGetUniformLocation(m_Shaders->ID, "mvp");
		m_CameraRightLoc = glGetUniformLocation(m_Shaders->ID, "cameraRight");
		m_CameraUpLoc = glGetUniformLocation(m_Shaders->ID, "cameraUp");
		m_texAtlasCellSizeLoc = glGetUniformLocation(m_Shaders->ID, "texAtlasCellSize");
		
		glBindVertexArray(0);
	}

	void PCParticleRenderData::LoadGPUData(const std::vector<ParticleAttribData>& a_InstanceData) const
	{
		glBindVertexArray(m_ParticleVAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_PositionsVBO);
		/*glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(float) * (3 + 4 + 2 + 1) * ParticleSystem::ms_MaxParticles,
			&a_InstanceData[0],
			GL_STREAM_DRAW);*/

		//buffer orphaning, a streaming optimization
		glBufferData(
			GL_ARRAY_BUFFER,
			sizeof(float) * (3 + 4 + 2 + 1) * ParticleSystem::ms_MaxParticles,
			NULL,
			GL_STREAM_DRAW);
		glBufferSubData(
			GL_ARRAY_BUFFER,
			0,
			sizeof(float) * (3 + 4 + 2 + 1) * ParticleSystem::ms_MaxParticles,
			&a_InstanceData[0]);

		glBindVertexArray(0);
	}

	void PCParticleRenderData::Draw(const glm::mat4 a_MVP, const glm::mat4 a_CameraView) const
	{
		m_ParticleSystem->SortParticlesBackToFront(a_MVP);
		LoadGPUData(m_ParticleSystem->GetInstanceBuffer());
		
		glBindVertexArray(m_ParticleVAO);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		m_Shaders->Use();

		const ParticleTexture& texture = m_Texture;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture.m_texture);
		
		m_Shaders->setMat4("mvp", a_MVP);
		const glm::vec3 cameraRight(a_CameraView[0][0], a_CameraView[1][0], a_CameraView[2][0]);
		m_Shaders->setVec3("cameraRight", cameraRight);
		const glm::vec3 cameraUp(a_CameraView[0][1], a_CameraView[1][1], a_CameraView[2][1]);
		m_Shaders->setVec3("cameraUp", cameraUp);
		m_Shaders->setVec2(
			"texAtlasCellSize", 
			glm::vec2(
				1.0f / static_cast<float>(m_ParticleSystem->GetParams().xTexDivisions),
				1.0f / static_cast<float>(m_ParticleSystem->GetParams().yTexDivisions)
			)
		);

		glDrawArraysInstanced(GL_TRIANGLES, 0, m_ParticleSystem->GetLiveParticleCount() * 6, m_ParticleSystem->GetLiveParticleCount());

		glDisable(GL_BLEND);
		glBindVertexArray(0);
	}

	void LoadTexture(ParticleTexture& a_Texture, char const* a_FilePath)
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
			switch(ncrChannels)
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
}
