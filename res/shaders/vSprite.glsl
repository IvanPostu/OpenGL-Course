#version 460

layout(location = 0) in vec2 vertex_position;
layout(location = 1) in vec2 texture_coords;

out vec2 texCoords;

uniform mat4 modelMat;
uniform mat4 projectionMat;

void main()
{
   texCoords = texture_coords;
   gl_Position = projectionMat * modelMat * vec4(vertex_position, 0.0, 1.0);
}