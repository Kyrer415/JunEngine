cmake_minimum_required(VERSION 3.15)

project(JuniorEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

# Собираем файлы (теперь тут только твои .cpp и .h файлы, никакого glad.c)
file(GLOB SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/ThirdParty/*.h"
)

add_executable(JuniorEngine
    JuniorEngine/Main.cpp  # <-- Добавили приставку папки к главному файлу!
    JuniorEngine/Engine/Core/CoreTime.h
    JuniorEngine/Engine/Core/CoreTime.cpp
    JuniorEngine/Engine/Core/input.h
    JuniorEngine/Engine/Core/input.cpp
    JuniorEngine/Engine/Core/Transform.h
    JuniorEngine/Engine/Window/Window.h
    JuniorEngine/Engine/Window/Window.cpp
    JuniorEngine/Engine/Renderer/Renderer.h
    JuniorEngine/Engine/Renderer/Renderer.cpp
    JuniorEngine/Engine/Renderer/Shader.h
    JuniorEngine/Engine/Renderer/Shader.cpp
    JuniorEngine/Engine/Renderer/Mesh.h
    JuniorEngine/Engine/Renderer/Mesh.cpp
    JuniorEngine/Engine/Renderer/VertexArray.h
    JuniorEngine/Engine/Renderer/VertexArray.cpp
    JuniorEngine/Engine/Renderer/Texture.h
    JuniorEngine/Engine/Renderer/Texture.cpp
    JuniorEngine/Engine/Renderer/Camera.h
    JuniorEngine/Engine/Renderer/Camera.cpp
    JuniorEngine/Engine/Renderer/Model.h
    JuniorEngine/Engine/Renderer/Model.cpp
)


target_include_directories(${PROJECT_NAME} PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/Engine"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/ThirdParty"
)

# Находим ВСЕ три библиотеки через vcpkg автоматикой
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(glad REQUIRED) # <-- Ищем Glad в vcpkg
find_package(imgui REQUIRED) # <-- тут ищем imgui!
find_package(assimp REQUIRED)

# Линкуем готовые бинарники vcpkg
target_link_libraries(${PROJECT_NAME} PRIVATE 
    opengl32
    glfw
    glm::glm
    imgui::imgui
    glad::glad # <-- Подключаем готовый скомпилированный Glad
    assimp::assimp
)

# ОЧИЩЕННЫЙ ТАРГЕТ С ПРАВИЛЬНЫМИ ПУТЯМИ (ДВА JuniorEngine)
add_custom_target(CopyAssets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/basic.vert" "${CMAKE_BINARY_DIR}/basic.vert"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/basic.frag" "${CMAKE_BINARY_DIR}/basic.frag"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/light.frag" "${CMAKE_BINARY_DIR}/light.frag"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/openGL_Logo.png" "${CMAKE_BINARY_DIR}/openGL_Logo.png"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/UnrealEngine_Logo.png" "${CMAKE_BINARY_DIR}/UnrealEngine_Logo.png"
    
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/models" "${CMAKE_BINARY_DIR}/models"

    COMMENT "Копирование шейдеров, текстур и 3D-моделей в папку сборки..."
)

add_dependencies(${PROJECT_NAME} CopyAssets)

