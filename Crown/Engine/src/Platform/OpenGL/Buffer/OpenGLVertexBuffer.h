#pragma once
#include "Core/Renderer/Buffer/VertexBuffer.h"

#include <memory>

namespace Crown
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* a_Vertices, uint32_t a_Count);
		virtual ~OpenGLVertexBuffer();
		void Bind() const override;
		void Unbind() const override;

	private:
		uint32_t m_RendererID;
	};
}
