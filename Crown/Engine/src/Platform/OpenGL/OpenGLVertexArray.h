#pragma once
#include "Core/Renderer/VertexArray.h"

namespace Crown
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray() = default;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void AddVertexBuffer(std::shared_ptr<VertexBuffer>& a_VertexBuffer) override;
		virtual void SetIndexBuffer(std::shared_ptr<IndexBuffer>& a_IndexBuffer) override;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const override { return m_VertexBuffers; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const override { return m_IndexBuffer; }

	private:
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_RendererId;
		
	};
}


