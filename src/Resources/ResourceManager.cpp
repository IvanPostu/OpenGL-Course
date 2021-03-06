#include "ResourceManager.h"
#include "../RenderEngine/ShaderProgram.h"
#include "../RenderEngine/Texture2D.h"
#include "../RenderEngine/Sprite.h"
#include "../RenderEngine/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms;
ResourceManager::TexturesMap ResourceManager::m_textures;
ResourceManager::SpritesMap ResourceManager::m_sprites;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animatedSprites;
std::string ResourceManager::m_path;

void ResourceManager::setExecutablePath(const std::string &executablePath)
{
  size_t found = executablePath.find_last_of("/\\");
  m_path = executablePath.substr(0, found);
}

void ResourceManager::unloadAllResources()
{
  m_shaderPrograms.clear();
  m_textures.clear();
  m_sprites.clear();
  m_animatedSprites.clear();
}

std::string ResourceManager::getFileString(const std::string &relativePath) 
{
  std::ifstream f;
  f.open(m_path + "/" + relativePath.c_str(), std::ios::binary);
  if (!f.is_open())
  {
    std::cerr << "Failed to open file: " << relativePath << std::endl;
    return std::string();
  }

  std::stringstream buffer;
  buffer << f.rdbuf();

  return buffer.str();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::loadShaders(const std::string &shaderName, const std::string &vertexPath, const std::string &fragmentPath)
{
  using namespace std;

  string vertexString = getFileString(vertexPath);
  if (vertexString.empty())
  {
    cerr << "No vertex shader!" << endl;
    return nullptr;
  }

  string fragmentString = getFileString(fragmentPath);
  if (fragmentString.empty())
  {
    cerr << "No fragment shader!" << endl;
    return nullptr;
  }

  shared_ptr<RenderEngine::ShaderProgram> &newShader = m_shaderPrograms.emplace(shaderName,
                                                                            make_shared<RenderEngine::ShaderProgram>(
                                                                                vertexString, fragmentString))
                                                       .first->second;

  if (newShader->isCompiled())
  {
    return newShader;
  }

  cerr << "Can't load shader program:" << endl
       << "Vertex: " << vertexPath << endl
       << "Fragment: " << fragmentPath << endl;

  return nullptr;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::getShaderProgram(
    const std::string &shaderName)
{
  using namespace std;

  ShaderProgramsMap::const_iterator it = m_shaderPrograms.find(shaderName);
  if (it != m_shaderPrograms.end())
  {
    return it->second;
  }

  cerr << "Can't find the shader program: " << shaderName << endl;
  return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTexture(const std::string &textureName,
                                                                  const std::string &texturePath)
{
  int channels = 0;
  int width = 0;
  int height = 0;

  stbi_set_flip_vertically_on_load(true);
  unsigned char *pixels = stbi_load(std::string(m_path + "/" + texturePath).c_str(),
                                    &width, &height, &channels, 0);

  if (!pixels)
  {
    std::cerr << "Can't load image " << texturePath << std::endl;
    return nullptr;
  }

  std::shared_ptr<RenderEngine::Texture2D> newTexture = m_textures
                                                        .emplace(
                                                            textureName,
                                                            std::make_shared<RenderEngine::Texture2D>(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE))
                                                        .first->second;

  stbi_image_free(pixels);

  return newTexture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::getTexture(const std::string &textureName)
{
  using namespace std;

  TexturesMap::const_iterator it = m_textures.find(textureName);
  if (it != m_textures.end())
  {
    return it->second;
  }

  cerr << "Can't find the texture: " << textureName << endl;
  return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::loadSprite(const std::string &spriteName,
                                                              const std::string &textureName,
                                                              const std::string &shaderName,
                                                              const unsigned int spriteWidth,
                                                              const unsigned int spriteHeight,
                                                              const std::string& subTextureName)
{
  auto pTexture = getTexture(textureName);
  if (!pTexture)
  {
    std::cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << std::endl;
  }

  auto pShader = getShaderProgram(shaderName);
  if (!pShader)
  {
    std::cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << std::endl;
  }

  std::shared_ptr<RenderEngine::Sprite> newSprite = m_sprites
                                                    .emplace(textureName,
                                                             std::make_shared<RenderEngine::
                                                                                  Sprite>(pTexture,
                                                                                          subTextureName,
                                                                                          pShader,
                                                                                          glm::vec2(0.f, 0.f),
                                                                                          glm::vec2(spriteWidth, spriteHeight)))
                                                    .first->second;

  return newSprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::getSprite(const std::string &spriteName)
{
  SpritesMap::const_iterator it = m_sprites.find(spriteName);
  if (it != m_sprites.end())
  {
    return it->second;
  }
  std::cerr << "Can't find the sprite: " << spriteName << std::endl;
  return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::loadTextureAtlas(std::string textureName,
                                                                       std::string texturePath,
                                                                       std::vector<std::string> subTextures,
                                                                       const unsigned int subTextureWidth,
                                                                       const unsigned int subTextureHeight)
{
  auto pTexture = loadTexture(std::move(textureName), std::move(texturePath));
  if(pTexture)
  {
    const unsigned int textureWidth = pTexture->width();
    const unsigned int textureHeight = pTexture->height();

    unsigned int currentTextureOffsetX = 0;
    unsigned int currentTextureOffsetY = textureHeight;

    for(auto& currentSubTextureName : subTextures)
    {
      glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth, 
        static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);

      glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, 
        static_cast<float>(currentTextureOffsetY) / textureHeight);

      pTexture->addSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

      currentTextureOffsetX += subTextureWidth;
      if(currentTextureOffsetX >= textureWidth)
      {
        currentTextureOffsetX = 0;
        currentTextureOffsetY -= subTextureHeight;
      }
    }
    
    
  }

  return pTexture;
}


std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::loadAnimatedSprite(const std::string& spriteName,
                                                                              const std::string& textureName,
                                                                              const std::string& shaderName,
                                                                              const unsigned int spriteWidth,
                                                                              const unsigned int spriteHeight,
                                                                              const std::string& subTextureName)
{
    auto pTexture = getTexture(textureName);
    if (!pTexture)
    {
        std::cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << std::endl;
    }

    auto pShader = getShaderProgram(shaderName);
    if (!pShader)
    {
        std::cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << std::endl;
    }

    std::shared_ptr<RenderEngine::AnimatedSprite> newSprite = m_animatedSprites.emplace(spriteName, std::make_shared<RenderEngine::AnimatedSprite>(pTexture,
        subTextureName,
        pShader,
        glm::vec2(0.f, 0.f),
        glm::vec2(spriteWidth, spriteHeight))).first->second;

    return newSprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::getAnimatedSprite(const std::string &spriteName)
{
  AnimatedSpritesMap::const_iterator it = m_animatedSprites.find(spriteName);
  if (it != m_animatedSprites.end())
  {
    return it->second;
  }
  std::cerr << "Can't find the animated sprite: " << spriteName << std::endl;
  return nullptr;
}
