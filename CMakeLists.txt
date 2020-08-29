
cmake_minimum_required(VERSION 3.13.4 FATAL_ERROR)

set(PROJECT_NAME OpenGLCourse)
set(CMAKE_BUILD_TYPE Debug)

project(${PROJECT_NAME})

message(STATUS "${PROJECT_NAME} build mode: ${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME} 
  src/Resources/stb_image.h
  
  src/main.cpp 

  src/Resources/ResourceManager.cpp
  src/Resources/ResourceManager.h

  src/RenderEngine/ShaderProgram.cpp 
  src/RenderEngine/ShaderProgram.h 
  src/RenderEngine/Texture2D.cpp
  src/RenderEngine/Texture2D.h
  src/RenderEngine/Sprite.cpp
  src/RenderEngine/Sprite.h
  src/RenderEngine/AnimatedSprite.cpp
  src/RenderEngine/AnimatedSprite.h
  src/RenderEngine/VertexBuffer.cpp
  src/RenderEngine/VertexBuffer.h
  src/RenderEngine/IndexBuffer.cpp
  src/RenderEngine/IndexBuffer.h
  src/RenderEngine/VertexArray.cpp
  src/RenderEngine/VertexArray.h
  src/RenderEngine/VertexBufferLayout.cpp
  src/RenderEngine/VertexBufferLayout.h
  src/RenderEngine/Renderer.cpp
  src/RenderEngine/Renderer.h

  src/Game/Game.cpp
  src/Game/Game.h
  src/Game/Tank.cpp
  src/Game/Tank.h
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)

include_directories(external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES 
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/ )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
