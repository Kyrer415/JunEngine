#include <iostream>
#include <cmath>

#include "Window.h"
#include "Input.h"
#include "Shader.h" 
#include "Renderer.h"
#include "Mesh.h"
#include "Time.h"
#include "Texture.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Engine Startup...\n";
    
    // Создаем подсистемы движка
    Window window(800, 600, "JuniorEngine via OOP");
    Renderer renderer;
    Shader ourShader("basic.vert", "basic.frag");

    // Задаем 4 вершины квадрата: координаты (X,Y,Z) + координаты текстуры (U,V)
    float vertices[] = {
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, // Точка 0: правый верхний угол (соответствует правому верхнему углу картинки)
         0.5f, -0.5f, 0.0f,   1.0f, 0.0f, // Точка 1: правый нижний угол (правый нижний угол картинки)
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, // Точка 2: левый нижний угол (левый нижний угол картинки)
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f  // Точка 3: левый верхний угол (левый верхний угол картинки)
    };

    // 2. наш орядок соединения углов
    unsigned int indices[] = {
        0, 1, 3, // первый триангл
        1, 2, 3 // второй триангл
    };



    // 3. Создаем объект меша одной строчкой, сразу передавая туда вершины!
    Mesh Square(vertices, sizeof(vertices), indices, sizeof(indices));

    // Создадим объект наей текстуры!
    Texture wallTexture("openGL_logo.png");

    float rotationAngle = 0.0f;

    // Главный цикл движка
    while (!window.ShouldClose())
    {
        // В самом начале кадра: обновляем Delta Time!
        Time::Update();

        // Обработка ввода: если нажат ESCAPE (код 256), закрываем движок!
        if (Input::IsKeyPressed(window, 256))
        {
            break;
        }


        renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

        // Включаем Шейдер
        ourShader.Use();

        // 1. Увеличиваем угол поворота: скорость * Delta Time!
        // 2.0f - это скорость вращения ( в радианах в секунду )
        rotationAngle += 2.0f * Time::GetDeltaTime();

        // 2. Передаём вычисленный угол в наш обновленный вершинный шейдер
        ourShader.SetFloat("u_Angle", rotationAngle);

        // Код польсации
        float timeValue = Time::GetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // Переводим синусоиду в диапозон от 0.0 до 1.0   
        ourShader.SetFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);  // Передаем плавно меняющийся цвет в шейдер через наш метод!

        // АКТИВИРУЕМ ТЕКСТУРУ ПЕРЕД ОТРИСОВКОЙ!
        wallTexture.Bind(0);

        // 4. Отрисовка одной командой!
        Square.Draw();

        window.Update();
    }

    return 0;
}
