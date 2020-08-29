#pragma once

#include <cstdlib>
#include <glad/glad.h>

namespace RenderEngine
{

  class IndexBuffer
  {
  public:
    IndexBuffer();
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer& ) = delete;
    IndexBuffer& operator =(const IndexBuffer&) = delete;

    IndexBuffer& operator=( IndexBuffer&& indexBuffer )noexcept;
    IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

    void init(const void* data, const size_t count);
    void bind() const;
    void unbind() const;
    size_t getCount() const { return m_count; }
  
  private:
    GLuint m_id;
    size_t m_count;
  };

}