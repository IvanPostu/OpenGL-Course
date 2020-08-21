#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"

GLfloat point[] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f};

GLfloat texCoord[] = {
    0.5f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f};

const char *vertex_shader = ""
  "#version 460\n"
  "layout(location = 0) in vec3 vertex_position;"
  "layout(location = 1) in vec3 vertex_color;"
  "out vec3 color;"
  "void main() {"
  "   color = vertex_color;"
  "   gl_Position = vec4(vertex_position, 1.0);"
  "}";

const char *fragment_shader = ""
  "#version 460\n"
  "in vec3 color;"
  "out vec4 frag_color;"
  "void main() {"
  "   frag_color = vec4(color, 1.0);"
  "}";

int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowResizeCallback(GLFWwindow *pWindow, int width, int height)
{
  g_windowSizeX = width;
  g_windowSizeY = height;
  glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }
}

int main(int argc, char **argv)
{
  using namespace std;
  

  if (!glfwInit())
  {
    cerr << "glfwInit failed!" << endl;
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "OpenGL Course", NULL, NULL);
  if (!pWindow)
  {
    cerr << "pWindow initialize error!" << endl;
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwSetWindowSizeCallback(pWindow, glfwWindowResizeCallback);
  glfwSetKeyCallback(pWindow, glfwKeyCallback);

  glfwMakeContextCurrent(pWindow);

  if (!gladLoadGL())
  {
    cerr << "Can't load GLAD !" << endl;
    return EXIT_FAILURE;
  }

  cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
  cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

  glClearColor(.2f, .5f, .5f, 1);

  {

    ResourceManager resourceManager(argv[0]);
    auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", 
      "res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
    if(!pDefaultShaderProgram)
    {
      cerr << "Can't create shader program: " << "DefaultShader" <<endl;
      return -1;
    }

    auto tex = resourceManager.loadTexture("DefaultTexture", "res/textures/map_16x16.png");

    GLuint points_vbo = 0;
    glGenBuffers(1, &points_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

    GLuint colors_vbo = 0;
    glGenBuffers(1, &colors_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    GLuint texCoord_vbo = 0;
    glGenBuffers(1, &texCoord_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texCoord_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    pDefaultShaderProgram->use();
    pDefaultShaderProgram->setInt("tex", 0);

    while (!glfwWindowShouldClose(pWindow))
    {
      glClear(GL_COLOR_BUFFER_BIT);

      // glUseProgram(shader_program);
      pDefaultShaderProgram->use();
      glBindVertexArray(vao);
      tex->bind();
      glDrawArrays(GL_TRIANGLES, 0, 3);

      /* Swap front and back buffers */
      glfwSwapBuffers(pWindow);
      /* Poll for and process events */
      glfwPollEvents();
    }

  }

  glfwTerminate();
  return EXIT_SUCCESS;
}