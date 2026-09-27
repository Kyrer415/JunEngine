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

# Линкуем готовые бинарники vcpkg
target_link_libraries(${PROJECT_NAME} PRIVATE 
    opengl32
    glfw
    glm::glm
    glad::glad # <-- Подключаем готовый скомпилированный Glad
)


# Железно копируем шейдеры и текстуры прямо в папку, где собирается .exe файл
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/basic.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/basic.frag"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/openGL_Logo.png"
    "${CMAKE_CURRENT_SOURCE_DIR}/JuniorEngine/UnrealEngine_Logo.png"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

