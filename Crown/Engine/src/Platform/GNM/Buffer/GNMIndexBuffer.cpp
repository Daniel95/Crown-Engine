#include "CrownPCH.h"
#include "GNMIndexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"

namespace Crown
{
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* a_Indices, uint32_t a_Count)
	{
		return std::make_shared<GNMIndexBuffer>(a_Indices, a_Count);
	}

    GNMIndexBuffer::GNMIndexBuffer(uint32_t* a_Indices, uint32_t a_Count)
        : m_Count(a_Count)
    {
        CROWN_PROFILE_FUNCTION();
  }

    GNMIndexBuffer::~GNMIndexBuffer()
    {
        CROWN_PROFILE_FUNCTION();

    }

    void GNMIndexBuffer::Bind() const
    {
        CROWN_PROFILE_FUNCTION();
    }

    void GNMIndexBuffer::Unbind() const
    {
        CROWN_PROFILE_FUNCTION();

    }
}
