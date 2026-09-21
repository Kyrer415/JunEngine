#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
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

    // Массив вершин куба: Координаты (X,Y,Z) + Текстурные координаты (U,V)
    float vertices[] = {
        // Задняя грань
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

        // Передняя грань
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

        // Левая грань
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        // Правая грань
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

         // Нижняя грань
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
          0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
          0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

         // Верхняя грань
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
          0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f
    };

    // Индексы для сборки 6 граней (каждая грань состоит из 2-х треугольников)
    unsigned int indices[] = {
         0,  1,  2,   2,  3,  0,
         4,  5,  6,   6,  7,  4,
         8,  9, 10,  10, 11,  8,
        12, 13, 14,  14, 15, 12,
        16, 17, 18,  18, 19, 16,
        20, 21, 22,  22, 23, 20
    };

    Mesh Cube(vertices, sizeof(vertices), indices, sizeof(indices));

    // Включаем ТЕСТ глубины для настойщего 3d!
    glEnable(GL_DEPTH_TEST);

    // Создадим объект наей текстуры!
    Texture wallTexture("openGL_logo.png");
    Texture faceTexture("UnrealEngine_Logo.png");

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

        ourShader.SetInt("texture1", 0); // Связываем texture1 со слотом 0
        ourShader.SetInt("texture2", 1); // Связываем texture2 со слотом 1


        // 1. Увеличиваем угол вращения на основе Delta Time
        rotationAngle += 1.0f * Time::GetDeltaTime();

        // 2. МАТРИЦА MODEL (Позиция, вращение и масштаб объекта в мире)
        glm::mat4 model = glm::mat4(1.0f); // создаём единичную матрицу
        // Вращаем куб по двум осям сразу (X и Y), чтобы видеть его объёмным!
        model = glm::rotate(model, rotationAngle, glm::vec3(0.5f, 1.0f, 0.0f));

        // 3. Матрица VIEW (Наша виртуальная камера)
        glm::mat4 view = glm::mat4(1.0f);
        // Отодвигаем "Камеру" назад на 3 единицы по оси Z, чтобы куб не был прямо в глазах
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -25.0f));

        // 4. МАТРИЦА PROJECTION ( Перспектива)
        // Параметры: угол обзора 45 градусов, соотношение сторон экрана 800\600, ближняя плоскость, дальняя плоскость
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        // 5. Закидываем все три матрицы на видеокарту через наш новый метод!
        ourShader.SetMatrix4("u_Model", model);
        ourShader.SetMatrix4("u_View", view);
        ourShader.SetMatrix4("u_Projection", projection);

        // Старый код пульсации цвета (our Color) 
        float TimeValue = Time::GetTime();
        float greenValue = (sin(TimeValue) / 2.0f) + 0.5f;
        ourShader.SetFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        // АКТИВИРУЕМ ТЕКСТУРУ ПЕРЕД ОТРИСОВКОЙ!
        wallTexture.Bind(0);
        faceTexture.Bind(1);

        // 4. Отрисовка одной командой!
        Cube.Draw();

        window.Update();
    }

    return 0;
}
