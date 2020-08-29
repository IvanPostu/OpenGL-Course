#pragma once

#include <glfw/glfw3.h>
#include <vector>

namespace RenderEngine
{
  struct BufferLayoutElement
  {
    GLint count;
    GLenum type;
    GLboolean normalized;
    size_t size;
  };

  class VertexBufferLayout
  {
  public:
    VertexBufferLayout();
    
    void addElementLayoutFloat(const unsigned int count, const bool normalized);
    void reserveElements(const size_t count);
    unsigned int getStride() const { return m_stride; }
    const std::vector<BufferLayoutElement>& getLayoutElements() const { return m_layoutElements; };
  private:
    std::vector<BufferLayoutElement> m_layoutElements;
    unsigned int m_stride;
  };
}