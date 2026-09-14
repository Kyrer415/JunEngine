#include <iostream>
#include <cmath>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Input.h"
#include "Shader.h" 
#include "Renderer.h"
#include "Mesh.h"


int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Engine Startup...\n";
    
    // Создаем подсистемы движка
    Window window(800, 600, "JuniorEngine via OOP");
    Renderer renderer;

    // Создаем шейдер 
    Shader ourShader("basic.vert", "basic.frag");

    // Сырые координаты
    float vertices[] = {
        -0.5f, -0.5f,  0.0f,
         0.5f, -0.5f,  0.0f,
         0.0f,  0.5f,  0.0f
    };

    // Создаем объект меша одной строчкой, сразу передавая туда вершины!
    Mesh Triangle(vertices, sizeof(vertices));

    // Главный цикл движка
    while (!window.ShouldClose())
    {

        // Обработка ввода: если нажат ESCAPE (код 256), закрываем движок!
        if (Input::IsKeyPressed(window, 256))
        {
            break;
        }


        renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

        // Включаем Шейдер
        ourShader.Use();

        // Динамически меняем цвет в зависисмотсти от времени работы приложения!
        // glfwGetTime() Возвращает время в секундах с момента старта
        float timeValue = (float)glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // Переводим синеусоиду в диапозон от 0.0 до 1.0   
        ourShader.SetFloat4("OurColor", 0.0f, greenValue, 0.0f, 1.0f);  // Передаем плавно меняющийся цвет в шейдер через наш новый метод!

        // 4. Отрисовка одной командой!
        Triangle.Draw();

        window.Update();
    }

    return 0;
}
