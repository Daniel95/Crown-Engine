#include "CrownPCH.h"

#include "ParticleSystem.h"

#include <stdlib.h>
#include <cassert>
#include <iterator>
#include <algorithm>

#include "ParticleRenderData.h"

namespace Crown
{
	const float ParticleSystem::ms_BillboardVertices[] = {
			 -0.5f, -0.5f, 0.0f, 0.f, 0.f,
			 0.5f, -0.5f, 0.0f, 1.f, 0.f,
			 -0.5f, 0.5f, 0.0f, 0.f, 1.f,

			 0.5f, -0.5f, 0.0f, 1.f, 0.f,
			 0.5f, 0.5f, 0.0f, 1.f, 1.f,
			 -0.5f, 0.5f, 0.0f, 0.f, 1.f
	};

	ParticleSystem::ParticleSystem()
	{
		m_ParticleSpawnCooldown = m_Params.spawnRate;
		
		m_Particles.resize(ms_MaxParticles);

		m_InstanceBuffer.resize(ms_MaxParticles);

		m_ParticleRenderData = ParticleRenderData::Create();
		m_ParticleRenderData->Init(this);
	}

	ParticleSystem::~ParticleSystem()
	{
		delete m_ParticleRenderData;
	}

	void ParticleSystem::Update(float a_DeltaTime)
	{
		TrySpawnParticles(a_DeltaTime);
		UpdateParticles(a_DeltaTime);
	}

	ParticleRenderData& ParticleSystem::GetParticleRenderData()
	{
		assert(m_ParticleRenderData);

		return *m_ParticleRenderData;
	}

	void ParticleSystem::SortParticlesBackToFront(const glm::mat4& a_MVP)
	{
		auto sortUntil = m_InstanceBuffer.begin();
		std::advance(sortUntil, m_LiveParticleCount);

		std::sort(m_InstanceBuffer.begin(), sortUntil, [=](const ParticleAttribData& a_1, const ParticleAttribData& a_2)
			{
				const glm::vec4 projCoord_1 = a_MVP * glm::vec4(a_1.position.x, a_1.position.y, a_1.position.z, 1.0f);
				const glm::vec3 ndc_1 = glm::vec3(
					projCoord_1.x / projCoord_1.w, 
					projCoord_1.y / projCoord_1.w,
					projCoord_1.z / projCoord_1.w
					);

				const glm::vec4 projCoord_2 = a_MVP * glm::vec4(a_2.position.x, a_2.position.y, a_2.position.z, 1.0f);
				const glm::vec3 ndc_2 = glm::vec3(
					projCoord_2.x / projCoord_2.w,
					projCoord_2.y / projCoord_2.w,
					projCoord_2.z / projCoord_2.w
				);
			
				return ndc_1.z > ndc_2.z;
			}
		);
	}

	size_t ParticleSystem::FindAvailableParticle()
	{
		for (size_t i = m_LastSpawnedParticleIndex; i < m_Particles.size(); ++i)
		{
			if (m_Particles[i].currentLife <= 0.0f)
			{
				m_LastSpawnedParticleIndex = i;
				return i;
			}
		}

		for (size_t i = 0; i < m_LastSpawnedParticleIndex; ++i)
		{
			if (m_Particles[i].currentLife <= 0)
			{
				m_LastSpawnedParticleIndex = i;
				return i;
			}
		}

		//if no particle found, then just return first particle (to be overwritten)
		return 0;
	}

	void ParticleSystem::TrySpawnParticles(float a_DeltaTime)
	{
		m_ParticleSpawnCooldown += a_DeltaTime;
		while(m_ParticleSpawnCooldown > 0)
		{
			const size_t newParticleIndex = FindAvailableParticle();
			SpawnParticle(newParticleIndex);
			
			m_ParticleSpawnCooldown -= m_Params.spawnRate;
		}
	}

	void ParticleSystem::SpawnParticle(size_t ParticleIndex)
	{
		ParticleData& p = m_Particles[ParticleIndex];

		RandomizeVec(p.attribData.position, m_Params.minPosition, m_Params.maxPosition, 3);
		
		p.maxLife = RandomFloat(m_Params.minLife, m_Params.maxLife);
		p.currentLife = p.maxLife;
		
		RandomizeVec(p.velocity, m_Params.minVelocity, m_Params.maxVelocity, 3);
		RandomizeVec(p.attribData.color, m_Params.minColor, m_Params.maxColor, 4);
		p.attribData.color.w *= m_Params.opacityMultiplier;
		p.attribData.scale = glm::vec2(1.0f, 1.0f)* RandomFloat(m_Params.minSize, m_Params.maxSize);

		p.attribData.rotation = RandomFloat(0.f, 3.14f);

		CalculateTexCoordOffset(
			p.attribData.texCoordOffset, 
			m_Params.xTexDivisions, 
			m_Params.yTexDivisions,
			p.currentLife / p.maxLife);
	}

	void ParticleSystem::UpdateParticles(float a_DeltaTime)
	{
		m_LiveParticleCount = 0;
		for (auto& p : m_Particles)
		{
			if (p.currentLife > 0.0f
				&& (p.currentLife -= a_DeltaTime) > 0.0f)
			{
				UpdateParticle(p, a_DeltaTime);

				m_InstanceBuffer[m_LiveParticleCount] = p.attribData;
				m_LiveParticleCount++;
			}
		}
	}

	void ParticleSystem::UpdateParticle(ParticleData& p, float a_DeltaTime)
	{
		p.velocity += m_Params.acceleration * a_DeltaTime;
		p.attribData.position += p.velocity * a_DeltaTime;

		if(m_Params.doesFadeOut)
		{
			p.attribData.color.w = (p.currentLife / p.maxLife) * m_Params.opacityMultiplier;
			if(p.attribData.color.w <= 0)
			{
				int a = 0;
				int b = a;
				
			}
		}
		
		CalculateTexCoordOffset(
			p.attribData.texCoordOffset, 
			m_Params.xTexDivisions,
			m_Params.yTexDivisions,
			p.currentLife / p.maxLife);
	}

	void CalculateTexCoordOffset(glm::vec2& offset, int texXDivisions, int texYDivisions, float progression)
	{				
		const float cellsCount = static_cast<float>(texXDivisions) * static_cast<float>(texYDivisions);
		const int currentCell = static_cast<int>(progression * cellsCount);
		
		offset.x = static_cast<float>(currentCell % texXDivisions) / static_cast<float>(texXDivisions);
		offset.y = static_cast<float>(currentCell / texXDivisions) / static_cast<float>(texYDivisions);
	}

	float RandomFloat(float a_Min, float a_Max)
	{
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float diff = a_Max - a_Min;
		const float r = random * diff;
		return a_Min + r;
	}

}