#pragma once

#include <cstdlib>
#include <glad/glad.h>

namespace RenderEngine
{

  class VertexBuffer
  {
  public:
    VertexBuffer();
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer& ) = delete;
    VertexBuffer& operator =(const VertexBuffer&) = delete;

    VertexBuffer& operator=( VertexBuffer&& vertexBuffer )noexcept;
    VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

    void init(const void* data, const size_t size);
    void update(const void* data, const size_t size) const;
    void bind() const;
    void unbind() const;
  
  private:
    GLuint m_id;
  };

}