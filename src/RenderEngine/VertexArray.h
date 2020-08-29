#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <cstdlib>
#include <glad/glad.h>


namespace RenderEngine
{

  class VertexArray
  {
  public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray& ) = delete;
    VertexArray& operator =(const VertexArray&) = delete;

    VertexArray& operator=( VertexArray&& vertexArray )noexcept;
    VertexArray(VertexArray&& vertexArray) noexcept;

    void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;
  
  private:
    GLuint m_id;
    size_t m_elementsCount;
  };

}