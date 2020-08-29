#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/vec2.hpp>

#include <iostream>
#include <chrono>
#include <cstdlib>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "RenderEngine/Renderer.h"

glm::ivec2 g_windowSize(640, 480);
Game g_game(g_windowSize);

void glfwWindowResizeCallback(GLFWwindow *pWindow, int width, int height)
{
  g_windowSize.x = width;
  g_windowSize.y = height;
  RenderEngine::Renderer::setViewport(width, height);
}

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }

  g_game.setKey(key, action);
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

  GLFWwindow *pWindow = glfwCreateWindow(g_windowSize.x, g_windowSize.y,
                                         "OpenGL Course", NULL, NULL);

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

  cout << "Renderer: " << RenderEngine::Renderer::getRendererStr() << endl;
  cout << "OpenGL version: " << RenderEngine::Renderer::getVersionStr() << endl;

  glClearColor(0.f, 0.f, 0.f, 1);

  {

    ResourceManager::setExecutablePath(argv[0]);
    g_game.init();

    auto lastTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(pWindow))
    {
      glfwPollEvents();
     
      auto currentTime = std::chrono::high_resolution_clock::now();
      uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
      lastTime = currentTime;
      g_game.update(duration);

      RenderEngine::Renderer::clear();

      g_game.render();

      glfwSwapBuffers(pWindow);
    }

    ResourceManager::unloadAllResources();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}