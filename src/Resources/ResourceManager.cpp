#include "ResourceManager.h"
#include "../Renderer/ShaderProgram.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath)
{
  size_t found = executablePath.find_last_of("/\\");
  m_path = executablePath.substr(0, found);
}

std::string ResourceManager::getFileString(const std::string& relativePath) const
{
  std::ifstream f;
  f.open(m_path + "/" + relativePath.c_str(), std::ios::binary);
  if(!f.is_open())
  {
    std::cerr << "Failed to open file: " << relativePath << std::endl;
    return std::string();
  }

  std::stringstream buffer;
  buffer << f.rdbuf();

  return buffer.str();
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::loadShaders(const std::string& shaderName,  const std::string& vertexPath, const std::string& fragmentPath )
{
  using namespace std;

  string vertexString = getFileString(vertexPath);
  if(vertexString.empty())
  {
    cerr << "No vertex shader!" << endl;
    return nullptr;
  }

  string fragmentString = getFileString(fragmentPath);
  if(fragmentString.empty())
  {
    cerr << "No fragment shader!" << endl;
    return nullptr;
  }

  shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms.emplace(shaderName, 
  make_shared<Renderer::ShaderProgram>(
    vertexString, fragmentString)).first->second;

  if(newShader->isCompiled()){
    return newShader;
  }

  cerr << "Can't load shader program:" << endl
    << "Vertex: " << vertexPath << endl
    << "Fragment: " << fragmentPath << endl;

  return nullptr;

}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::getShaderProgram(const std::string& shaderName)
{
  using namespace std;

  ShaderProgramMap::const_iterator it = m_shaderPrograms.find(shaderName);
  if(it != m_shaderPrograms.end())
  {
    return it->second;
  }

  cerr << "Can't find the shader program: " << shaderName << endl;
  return nullptr;
}

void ResourceManager::loadTexture(const std::string& textureName, const std::string& texturePath)
{
  int channels = 0;
  int width = 0;
  int height = 0;

  unsigned char* pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(), 
    &width, &height, &channels, 0);

  if(!pixels)
  {
    std::cerr << "Can't load image " << texturePath <<std::endl;
    return;
  }

  stbi_image_free(pixels);
  
}
