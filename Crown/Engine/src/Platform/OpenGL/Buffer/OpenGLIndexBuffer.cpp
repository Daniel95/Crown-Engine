#include "CrownPCH.h"
#include "OpenGLIndexBuffer.h"
#include "Core/Renderer/Buffer/IndexBuffer.h"

#include "glad/glad.h"

namespace Crown
{
	std::shared_ptr<IndexBuffer> IndexBuffer::Create(uint32_t* a_Indices, uint32_t a_Count)
	{
		return std::make_shared<OpenGLIndexBuffer>(a_Indices, a_Count);
	}

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* a_Indices, uint32_t a_Count)
        : m_Count(a_Count)
    {
        CROWN_PROFILE_FUNCTION();

        glGenBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, a_Count * sizeof(uint32_t), a_Indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        CROWN_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        CROWN_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void OpenGLIndexBuffer::Unbind() const
    {
        CROWN_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
