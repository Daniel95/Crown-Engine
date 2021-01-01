#pragma once

#include <memory>

namespace Crown
{
	class IndexBuffer
	{
	public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static std::shared_ptr<IndexBuffer> Create(uint32_t * a_Indices, uint32_t a_Count);
	};
}
