#pragma once

#include <string>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

namespace RenderEngine
{
  class Renderer
  {
  public:
    static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader);
    static void setClearColor(float r, float g, float b, float a);
    static void clear();
    static void setViewport(uint32_t width, uint32_t height, uint32_t leftOffset = 0, uint32_t bottomOffset = 0 );
    static std::string getRendererStr();
    static std::string getVersionStr();
  };

}