#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

namespace Crown
{
	struct JointData;
	class Joint
	{
	public:
		Joint(const std::vector<std::shared_ptr<JointData>>& a_JointData, size_t a_JointIndex, Joint* a_Parent);
		~Joint();

		void UpdateBoneMatrices(std::vector<glm::mat4>& a_BoneMatrices, glm::mat4 a_Model, glm::mat4* a_ParentTransform);

		Joint* FindJointByIndex(size_t a_Index);
		Joint* FindJointByName(std::string a_Name);
		
	private:

	private:
		friend class Animation;
		
		size_t m_Index = -1;
		std::string m_Name;
		std::vector<Joint> m_Children;
		Joint* m_Parent = nullptr;
		
		glm::vec3 m_DefaultPos;
		glm::vec4 m_DefaultRotation;
		glm::vec3 m_DefaultScale;
		glm::mat4 m_GlobalTransform;
		
		glm::mat4 m_LocalTransform;

		//position, rotation and scale of joint in world space but inverted (to go from local bone space to model space)
		glm::mat4 m_InverseBindTransform;
	};
}
