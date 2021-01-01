#include "CrownPCH.h"
#include "GNMVertexBuffer.h"
#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Crown
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* a_Vertices, uint32_t a_Count)
	{
		return std::make_shared<GNMVertexBuffer>(a_Vertices, a_Count);
	}

    GNMVertexBuffer::GNMVertexBuffer(float* a_Vertices, uint32_t a_Count)
    {
        CROWN_PROFILE_FUNCTION();
    }

    GNMVertexBuffer::~GNMVertexBuffer()
    {
        CROWN_PROFILE_FUNCTION();
    }

    void GNMVertexBuffer::Bind() const
    {
        CROWN_PROFILE_FUNCTION();
    }

    void GNMVertexBuffer::Unbind() const
    {
        CROWN_PROFILE_FUNCTION();
    }
}
