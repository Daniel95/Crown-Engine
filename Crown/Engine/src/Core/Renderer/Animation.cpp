#include "CrownPCH.h"

#include <glm/gtx/quaternion.hpp>

#include "Animation.h"
#include "Joint.h"

#include "GLTFLoading/GLTFParser.h"

namespace Crown
{
	Animation::Animation(AnimationData& a_AnimationData)
	{
		m_Name = a_AnimationData.name;
		
		m_ChannelData = a_AnimationData.channelData;
		m_SamplerData = a_AnimationData.samplerData;

		m_AnimationDuration = m_SamplerData[0]->inputs.back();
	}

	Animation::~Animation()
	{
		
	}

	void Animation::Update(Joint& a_RootJoint, float a_DeltaTime)
	{
		m_TimeElapsed += a_DeltaTime;
		m_TimeElapsed = fmodf(m_TimeElapsed, m_AnimationDuration);
		
		for(auto& channelData : m_ChannelData)
		{			
			Joint* joint = a_RootJoint.FindJointByIndex(channelData->jointIndex);
			assert(joint != nullptr);

			size_t beforeKeyIndex = -1;
			size_t afterKeyIndex = -1;
			float progression = 0.f;
			
			if(channelData->samplerDataPositionIndex >= 0)
			{
				GetSurroundingKeyFrameIndices(
					*m_SamplerData[channelData->samplerDataPositionIndex], 
					beforeKeyIndex, 
					afterKeyIndex,
					progression);

				glm::vec3 beforePos = m_SamplerData[channelData->samplerDataPositionIndex]->vec3Outputs[beforeKeyIndex];
				glm::vec3 afterPos = m_SamplerData[channelData->samplerDataPositionIndex]->vec3Outputs[afterKeyIndex];

				glm::vec3 finalPos = beforePos + (afterPos - beforePos) * progression;
				
				joint->m_DefaultPos = /*beforePos*/finalPos;
			}
			else if (channelData->samplerDataRotationIndex >= 0)
			{
				GetSurroundingKeyFrameIndices(
					*m_SamplerData[channelData->samplerDataRotationIndex],
					beforeKeyIndex,
					afterKeyIndex,
					progression);

				glm::vec4 beforeRot = m_SamplerData[channelData->samplerDataRotationIndex]->vec4Outputs[beforeKeyIndex];
				glm::quat beforeRotQuat; beforeRotQuat.x = beforeRot.x; beforeRotQuat.y = beforeRot.y; beforeRotQuat.z = beforeRot.z; beforeRotQuat.w = beforeRot.w;
				
				glm::vec4 afterRot = m_SamplerData[channelData->samplerDataRotationIndex]->vec4Outputs[afterKeyIndex];
				glm::quat afterRotQuat; afterRotQuat.x = afterRot.x; afterRotQuat.y = afterRot.y; afterRotQuat.z = afterRot.z; afterRotQuat.w = afterRot.w;

				glm::quat finalRot = glm::mix(beforeRotQuat, afterRotQuat, progression);
				
				joint->m_DefaultRotation = /*beforeRot*/glm::vec4(finalRot.x, finalRot.y, finalRot.z, finalRot.w);
			}
			else if (channelData->samplerDataScaleIndex >= 0)
			{
				GetSurroundingKeyFrameIndices(
					*m_SamplerData[channelData->samplerDataScaleIndex],
					beforeKeyIndex,
					afterKeyIndex,
					progression);

				glm::vec3 beforeScale = m_SamplerData[channelData->samplerDataScaleIndex]->vec3Outputs[beforeKeyIndex];
				glm::vec3 afterScale = m_SamplerData[channelData->samplerDataScaleIndex]->vec3Outputs[afterKeyIndex];
				
				joint->m_DefaultScale = beforeScale;
			}
			else
			{
				assert(false);
			}
		}
	}

	void Animation::GetSurroundingKeyFrameIndices(SamplerData& a_SamplerData, size_t& a_BeforeKeyIndex, size_t& a_AfterKeyIndex, float& a_Progression)
	{
		bool hit = false;
		float beforeKeyTime = 0.0f;
		float afterKeyTime = 0.0f;
		
		for(int i = 0; i < a_SamplerData.inputs.size(); ++i)
		{
			if(a_SamplerData.inputs[i] >= m_TimeElapsed)
			{
				if(i == 0)
				{
					a_BeforeKeyIndex = a_SamplerData.inputs.size() - 1;
					beforeKeyTime = 0.f;
				}
				else
				{
					a_BeforeKeyIndex = i - 1;
					beforeKeyTime = a_SamplerData.inputs[a_BeforeKeyIndex];
				}
				
				a_AfterKeyIndex = i;
				afterKeyTime = a_SamplerData.inputs[a_AfterKeyIndex];

				hit = true;
				a_Progression = (m_TimeElapsed - beforeKeyTime) / (afterKeyTime - beforeKeyTime);

				assert(a_Progression >= 0.f && a_Progression <= 1.f);
				
				break;
			}
		}
		assert(hit);
	}
}
