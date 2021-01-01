#include "CrownPCH.h"
#include "OpenGLVertexBuffer.h"
#include "Core/Renderer/Buffer/VertexBuffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include "glad/glad.h"

namespace Crown
{
	std::shared_ptr<VertexBuffer> VertexBuffer::Create(float* a_Vertices, uint32_t a_Count)
	{
		return std::make_shared<OpenGLVertexBuffer>(a_Vertices, a_Count);
	}

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* a_Vertices, uint32_t a_Count)
    {
        CROWN_PROFILE_FUNCTION();

        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, a_Count * sizeof(float), a_Vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        CROWN_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        CROWN_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLVertexBuffer::Unbind() const
    {
        CROWN_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
