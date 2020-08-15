#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>

int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowResizeCallback(GLFWwindow* pWindow, int width, int height)
{
  g_windowSizeX = width;
  g_windowSizeY = height;
  glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode)
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(pWindow, GL_TRUE);
  }

}

int main(int argc, char** argv)
{
  using namespace std;

  if (!glfwInit())
  {
    cerr << "glfwInit failed!" << endl;
    return EXIT_FAILURE;
  }

  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  if(!gladLoadGL()){
    cerr << "Can't load GLAD !" << endl;
    return EXIT_FAILURE;
  }

  cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
  cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

  glClearColor(.5f, .2f, 0, 1);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(pWindow))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(pWindow);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}