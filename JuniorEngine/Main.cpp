#include <iostream>
#include "Window.h"
#include "Renderer.h"
#include "Shader.h" // 1. Подключаем наш класс шейдеров
#include <glad/glad.h>

int main()
{
    std::cout << "Engine Startup...\n";

    // Создаем подсистемы движка
    Window window(800, 600, "JuniorEngine via OOP");
    Renderer renderer;

    // 2. Создаем шейдер одной строчкой! Передаем пути к файлам на диске
    Shader ourShader("basic.vert", "basic.frag");

    // ГЕОМЕТРИЯ ТРЕУГОЛЬНИКА (Пока оставляем буферы здесь)
    float vertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Главный цикл движка
    while (!window.ShouldClose())
    {
        renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

        // 3. Вместо glUseProgram(shaderProgram) активируем наш класс!
        ourShader.Use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.Update();
    }

    // Очистка буферов при выходе (тоже потом спрячем)
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}
