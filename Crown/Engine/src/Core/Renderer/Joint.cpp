#include "CrownPCH.h"

#include "Core/Renderer/Joint.h"

#include <cassert>

#include <Core/Renderer/GLTFLoading/GLTFParser.h>
#include <glm/gtx/quaternion.hpp>
#include <Platform/PC/Renderer/PCRenderer.h>

namespace Crown
{
	Joint::Joint(const std::vector<std::shared_ptr<JointData>>& a_JointData, size_t a_JointIndex, Joint* a_Parent)
	{
		m_Index = a_JointIndex;
		m_Name = a_JointData[m_Index]->name;
		m_Parent = a_Parent;
		m_InverseBindTransform = a_JointData[m_Index]->matrix;
		
		m_DefaultPos = a_JointData[m_Index]->position;
		m_DefaultRotation = a_JointData[m_Index]->rotation;
		m_DefaultScale = a_JointData[m_Index]->scale;
		
		for(const auto& childIndex : a_JointData[m_Index]->children)
		{
			m_Children.emplace_back(a_JointData, static_cast<size_t>(childIndex), this);
		}
	}

	Joint::~Joint()
	{
	}

	void Joint::UpdateBoneMatrices(std::vector<glm::mat4>& a_BoneMatrices, glm::mat4 a_Model, glm::mat4* a_ParentTransform) //TODO: turn into reference
	{	
		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.f), m_DefaultPos);
		
		glm::quat rotationQuat;
		rotationQuat.x = m_DefaultRotation.x;
		rotationQuat.y = m_DefaultRotation.y;
		rotationQuat.z = m_DefaultRotation.z;
		rotationQuat.w = m_DefaultRotation.w;
		
		glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), m_DefaultScale);

		m_LocalTransform = translationMatrix * rotationMatrix * scaleMatrix;

		if(a_ParentTransform == nullptr)
		{
			m_GlobalTransform = a_Model * m_LocalTransform;
		}
		else
		{
			m_GlobalTransform = *(a_ParentTransform) * m_LocalTransform;
		}
		
		a_BoneMatrices[m_Index] =
			glm::inverse(a_Model) *
			m_GlobalTransform *
			m_InverseBindTransform;
		
		for (auto& child : m_Children)
		{
			child.UpdateBoneMatrices(a_BoneMatrices, a_Model, &m_GlobalTransform);
		}
	}

	Joint* Joint::FindJointByIndex(size_t a_Index)
	{		
		if(m_Index == a_Index)
		{
			return this;
		}

		for (auto& child : m_Children)
		{
			Joint* childResult =  child.FindJointByIndex(a_Index);
			if(childResult != nullptr)
			{
				return childResult;
			}
		}

		return nullptr;
	}

	Joint* Joint::FindJointByName(std::string a_Name)
	{
		if (m_Name == a_Name)
		{
			return this;
		}

		for (auto& child : m_Children)
		{
			Joint* childResult = child.FindJointByName(a_Name);
			if (childResult != nullptr)
			{
				return childResult;
			}
		}

		return nullptr;
	}
}
