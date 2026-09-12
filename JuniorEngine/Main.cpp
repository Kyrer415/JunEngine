#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Shader.h" 
#include "Renderer.h"
#include "VertexArray.h"


int main()
{
    setlocale(LC_ALL, "Russian");

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

    // 3. Создаем объект нашего класса и загружаем данные одной строчкой! ^~^
    VertexArray TriangleMesh;
    TriangleMesh.SetData(vertices, sizeof(vertices));

    // Главный цикл движка
    while (!window.ShouldClose())
    {
        renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

        // 3. Вместо glUseProgram(shaderProgram) активируем наш класс!
        ourShader.Use();

        // Динамически меняем цвет в зависисмотсти от времени работы приложения!
        // glfwGetTime() Возвращает время в секундах с момента старта
        float timeValue = (float)glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // Переводим синеусоиду в диапозон от 0.0 до 1.0   
        ourShader.SetFloat4("OurColor", 0.0f, greenValue, 0.0f, 1.0f);  // Передаем плавно меняющийся цвет в шейдер через наш новый метод!

        // 4. Отрисовка через ООП! Включаем буфер и отдаём команду видеокарте
        TriangleMesh.Bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        window.Update();
    }

    return 0;
}
