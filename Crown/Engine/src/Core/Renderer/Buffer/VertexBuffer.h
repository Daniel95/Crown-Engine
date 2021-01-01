#pragma once

#include <memory>

namespace Crown
{
    class VertexBuffer
    {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static std::shared_ptr<VertexBuffer> Create(float* a_Vertices, uint32_t a_Count);
    };
}
