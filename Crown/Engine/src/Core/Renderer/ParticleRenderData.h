#pragma once

#include <vector>

namespace Crown
{
	struct ParticleAttribData;
	
	class ParticleSystem;
	
	class ParticleRenderData
	{
	public:
		virtual ~ParticleRenderData() = default;

		virtual void Init(ParticleSystem* a_ParticleSystem) = 0;
		
		virtual void LoadGPUData(const std::vector<ParticleAttribData>& a_InstanceData) const = 0;
		
		static ParticleRenderData* Create();
		
	protected:
		ParticleSystem* m_ParticleSystem = nullptr;
	};
}
