#include "CrownPCH.h"
#include "GNMVertexArray.h"
#include "Core/Renderer/GLTFLoading/GLTFParser.h"

namespace Crown
{
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	std::shared_ptr<VertexArray> VertexArray::Create()
	{
		return std::make_shared<GNMVertexArray>();
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

	GNMVertexArray::GNMVertexArray()
	{
		CROWN_PROFILE_FUNCTION();

		//glCreateVertexArrays(1, &m_RendererId);
	}

	void GNMVertexArray::Bind() const
	{
		CROWN_PROFILE_FUNCTION();

	}

	void GNMVertexArray::Unbind() const
	{
		CROWN_PROFILE_FUNCTION();
	}

	void GNMVertexArray::AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer)
	{
		CROWN_PROFILE_FUNCTION();


	}

	void GNMVertexArray::SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer)
	{
		CROWN_PROFILE_FUNCTION();

	}

}
