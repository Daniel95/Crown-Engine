#pragma once

#include <vector>
#include <memory>

#include "GLTFLoading/GLTFParser.h"

namespace Crown
{
	struct AnimationData;
	struct SamplerData;
	class Joint;
	
	class Animation
	{
	public:
		Animation(AnimationData& a_AnimationData);
		~Animation();

		void Update(Joint& a_RootJoint, float a_DeltaTime);

		std::string GetName() const { return m_Name; }
		
	private:
		void GetSurroundingKeyFrameIndices(SamplerData& a_SamplerData, size_t& a_BeforeKeyIndex, size_t& a_AfterKeyIndex, float& a_Progression);
		
	private:
		float m_TimeElapsed = 0.0f;
		float m_AnimationDuration = 0.0f;
		
		std::vector<std::shared_ptr<ChannelData>> m_ChannelData;
		std::vector<std::shared_ptr<SamplerData>> m_SamplerData;
		std::string m_Name;
	};
}
