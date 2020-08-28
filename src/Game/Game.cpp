
#include "Game.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/ShaderProgram.h"
#include "../Resources/ResourceManager.h"
#include "../Renderer/Texture2D.h"
#include "../Renderer/Sprite.h"
#include "../Renderer/AnimatedSprite.h"

#include "Tank.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

Game::Game(const glm::ivec2 &windowSize)
    : m_eCurrentGameState(EGameState::ACTIVE),
      m_windowSize(windowSize)
{
  m_keys.fill(false);
}

Game::~Game()
{
}

void Game::render()
{
  if (m_pTank)
  {
    m_pTank->render();
  }
  // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->render();
}

void Game::update(const uint64_t delta)
{

  if (m_pTank)
  {
    if(m_keys[GLFW_KEY_W])
    {
      m_pTank->setOrientation(Tank::EOrientation::Top);
      m_pTank->move(true);
    }
    else if (m_keys[GLFW_KEY_S])
    {
      m_pTank->setOrientation(Tank::EOrientation::Bottom);
      m_pTank->move(true);
    }
    else if (m_keys[GLFW_KEY_A])
    {
      m_pTank->setOrientation(Tank::EOrientation::Left);
      m_pTank->move(true);
    }
    else if (m_keys[GLFW_KEY_D])
    {
      m_pTank->setOrientation(Tank::EOrientation::Right);
      m_pTank->move(true);
    }
    else
    {
      m_pTank->move(false);
    }




    m_pTank->update(delta);
  }
  // ResourceManager::getAnimatedSprite("NewAnimatedSprite")->update(delta);
}

void Game::setKey(const int key, const int action)
{
  m_keys[key] = action;
}

bool Game::init()
{
  using namespace std;

  auto pDefaultShaderProgram = ResourceManager::loadShaders("DefaultShader",
                                                            "res/shaders/vertex.glsl", "res/shaders/fragment.glsl");
  if (!pDefaultShaderProgram)
  {
    cerr << "Can't create shader program: "
         << "DefaultShader" << endl;
    return false;
  }

  auto pSpriteShaderProgram = ResourceManager::loadShaders("SpriteShader",
                                                           "res/shaders/vSprite.glsl", "res/shaders/fSprite.glsl");
  if (!pDefaultShaderProgram)
  {
    cerr << "Can't create SpriteShader program: " << endl;
    return false;
  }

  auto tex = ResourceManager::loadTexture("DefaultTexture", "res/textures/map_16x16.png");

  std::vector<std::string> subTexturesNames = {
      "block",
      "topBlock",
      "bottomBlock",
      "leftBlock",
      "rightBlock",
      "topLeftBlock",
      "topRightBlock",
      "bottomLeftBlock",

      "bottomRightBlock",
      "beton",
      "topBeton",
      "bottomBeton",
      "leftBeton",
      "rightBeton",
      "topLeftBeton",
      "topRightBeton",

      "bottomLeftBeton",
      "bottomRightBeton",
      "water1",
      "water2",
      "water3",
      "trees",
      "ice",
      "wall",

      "eagle",
      "deadEagle",
      "nothing",
      "respawn1",
      "respawn2",
      "respawn3",
      "respawn4"};

  auto pTextureAtlas = ResourceManager::loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png",
                                                         std::move(subTexturesNames), 16, 16);

  auto pAnimatedSprite = ResourceManager::loadAnimatedSprite(
      "NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "beton");

  pAnimatedSprite->setPosition(glm::vec2(300, 300));
  std::vector<std::pair<std::string, uint64_t>> waterState;
  waterState.emplace_back(std::make_pair<std::string, uint64_t>("water1", 1e9));
  waterState.emplace_back(std::make_pair<std::string, uint64_t>("water2", 1e9));
  waterState.emplace_back(std::make_pair<std::string, uint64_t>("water3", 1e9));

  std::vector<std::pair<std::string, uint64_t>> eagleState;
  eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1e9));
  eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1e9));

  pAnimatedSprite->insertState("waterState", move(waterState));
  pAnimatedSprite->insertState("eagleState", move(eagleState));

  pAnimatedSprite->setState("waterState");

  pDefaultShaderProgram->use();
  pDefaultShaderProgram->setInt("tex", 0);

  glm::mat4 modelMatrix_1 = glm::mat4(1.f);
  modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 50.f, 0));

  glm::mat4 modelMatrix_2 = glm::mat4(1.f);
  modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(590.f, 50.f, 0));

  glm::mat4 projectionMatrix = glm::ortho(
      0.f, static_cast<float>(m_windowSize.x), 0.f, static_cast<float>(m_windowSize.y), -100.f, 100.f);

  pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

  pSpriteShaderProgram->use();
  pSpriteShaderProgram->setInt("tex", 0);
  pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

  std::vector<std::string> tankSubTexturesNames = {
      "tankTop1",
      "tankTop2",
      "tankLeft1",
      "tankLeft2",
      "tankBottom1",
      "tankBottom2",
      "tankRight1",
      "tankRight2"};

  auto pTanksTextureAtlas = ResourceManager::loadTextureAtlas("TanksTextureAtlas", "res/textures/tanks.png",
                                                              std::move(tankSubTexturesNames), 16, 16);

  auto pTanksAnimatedSprite = ResourceManager::loadAnimatedSprite(
      "TanksAnimatedSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, "tankTop1");

  std::vector<std::pair<std::string, uint64_t>> tankTopState;
  tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop1", 5 * 1e8));
  tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop2", 5 * 1e8));

  std::vector<std::pair<std::string, uint64_t>> tankBottomState;
  tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom1", 5 * 1e8));
  tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom2", 5 * 1e8));

  std::vector<std::pair<std::string, uint64_t>> tankRightState;
  tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight1", 5 * 1e8));
  tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight2", 5 * 1e8));

  std::vector<std::pair<std::string, uint64_t>> tankLeftState;
  tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft1", 5 * 1e8));
  tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft2", 5 * 1e8));

  pTanksAnimatedSprite->insertState("tankTopState", std::move(tankTopState));
  pTanksAnimatedSprite->insertState("tankBottomState", std::move(tankBottomState));
  pTanksAnimatedSprite->insertState("tankLeftState", std::move(tankLeftState));
  pTanksAnimatedSprite->insertState("tankRightState", std::move(tankRightState));

  pTanksAnimatedSprite->setState("tankTopState");
  m_pTank = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));
  return true;
}
