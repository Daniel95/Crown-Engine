#include "CrownPCH.h"
#include "OpenGLVertexArray.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"

namespace Crown
{
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<OpenGLVertexArray>();
	}

	std::shared_ptr<VertexArray> VertexArray::Create(float* a_Vertices, uint32_t a_VerticesCount, uint32_t* a_Indices, uint32_t a_IndicesCount)
	{
		std::shared_ptr<VertexArray> va = Create();

		std::shared_ptr<VertexBuffer> vb = VertexBuffer::Create(a_Vertices, a_VerticesCount);
		vb->Bind();

		va->AddVertexBuffer(vb);

		std::shared_ptr<IndexBuffer> ib = IndexBuffer::Create(a_Indices, a_IndicesCount);
		ib->Bind();
		
		va->SetIndexBuffer(ib);

		return va;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		CROWN_PROFILE_FUNCTION();

		glGenVertexArrays(1, &m_RendererId);
		
		//glCreateVertexArrays(1, &m_RendererId);
	}

	void OpenGLVertexArray::Bind() const
	{
		CROWN_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererId);
	}

	void OpenGLVertexArray::Unbind() const
	{
		CROWN_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer)
	{
		CROWN_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererId);

		a_VertexBuffer->Bind();
		
		//Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(
			0,						//vertex pos location in shader
			3,						//size of vertex pos data
			GL_FLOAT,				//position data is in floats (vec3)
			GL_FALSE,				//dont normalize
			sizeof(Vertex),			//stride to next position
			nullptr);

		//Normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			BUFFER_OFFSET(sizeof(Vertex::pos))
		);

		//Textures
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(
			2,
			2,
			GL_FLOAT,
			GL_FALSE,
			sizeof(Vertex),
			//BUFFER_OFFSET(sizeof(float) * 5)
			BUFFER_OFFSET((sizeof(Vertex::pos) + sizeof(Vertex::normal)))
		);

		m_VertexBuffers.push_back(a_VertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer)
	{
		CROWN_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererId);
		a_IndexBuffer->Bind();
		m_IndexBuffer = a_IndexBuffer;
	}

}
