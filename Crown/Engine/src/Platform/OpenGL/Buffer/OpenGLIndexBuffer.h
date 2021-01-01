#pragma once
#include "Core/Renderer/Buffer/IndexBuffer.h"

#include <memory>

namespace Crown
{
	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* a_Indices, uint32_t a_Count);
		~OpenGLIndexBuffer() override;
		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual uint32_t GetCount() const override { return m_Count; }
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
		
	};
}
