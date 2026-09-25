#include <glad/glad.h>
#include <glm/glm.hpp>
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
#include "Camera.h" // подключили камеру

int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Engine Startup...\n";
    
    // Создаем подсистемы движка
    Window window(1600, 1200, "JuniorEngine via OOP");
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

    // Включаем Шейдер
    ourShader.Use();

    ourShader.SetInt("texture1", 0); // Связываем texture1 со слотом 0
    ourShader.SetInt("texture2", 1); // Связываем texture2 со слотом 1

    glm::vec3 CubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // Создаём объект камеры
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
    float rotationAngle = 0.0f;

    // 2. Прячем курсор мыши и запираем в окне
    window.DisableCursor();

    // 3. Перемещаем длдя отслеживания перемещеия мыши
    // Изначально ставим их в центр экрана (800x600 -> 400x300)
    float lastX = window.GetWidth() / 2.0f;
    float lastY = window.GetHeight() / 2.0f;
    bool firstMouse = true; // Флаг, чтобы избежать дикого скачка камеры при первом кадре

    // Главный цикл движка
    while (!window.ShouldClose())
    {
        // В самом начале кадра: обновляем Delta Time!
        Time::Update();

        // Обработка ввода: если нажат ESCAPE (код 256), закрываем движок!
        if (Input::IsKeyPressed(window, 256))
        {
            window.Close();
        }

        // 3. Обновляем позицию камеры на основе клавиатуры и DeltaTime!
        camera.ProcessInput(window, Time::GetDeltaTime());

        // 5. Обработка мыши
        double mouseX, mouseY;
        // запрашиваем у GLFW координаты курсора
        glfwGetCursorPos(window.GetNativeWindow(), &mouseX, &mouseY);

        // Если это самый первый кадр, просто запомним позицию без рывка камеры
        if (firstMouse)
        {
            lastX = (float)mouseX;
            lastY = (float)mouseY;
            firstMouse = false;
        }

        // ССчитаем смещение мыши между текущим и прошлым кадром
        float xOffset = (float)mouseX - lastX;
        // Инвертируем Y, так как в GLFW координаты экарна идут сверху вниз, а в 3D снизу вверх
        float yOffset = lastY - (float)mouseY;

        // Запоминаем текущие координаты как "Прошлые" для след. кадра
        lastX = (float)mouseX;
        lastY = (float)mouseY;

        // Передаём дельту перемещения в класс камеры
        camera.ProcessMouseMovement(xOffset, yOffset);

        renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

        ourShader.Use();

        // 1. Увеличиваем угол вращения на основе Delta Time
        rotationAngle += 1.0f * Time::GetDeltaTime();

        // 2. МАТРИЦА MODEL (Позиция, вращение и масштаб объекта в мире)
        glm::mat4 model = glm::mat4(1.0f); // создаём единичную матрицу
        // Вращаем куб по двум осям сразу (X и Y), чтобы видеть его объёмным!
        model = glm::rotate(model, rotationAngle, glm::vec3(0.5f, 1.0f, 0.0f));

        // 3. Матрица VIEW (Наша виртуальная камера)
        glm::mat4 view = camera.GetViewMatrix();

        // 4. МАТРИЦА PROJECTION ( Перспектива)
        // Параметры: угол обзора 45 градусов, соотношение сторон экрана , ближняя плоскость, дальняя плоскость
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.GetWidth() / (float)window.GetHeight(), 0.1f, 100.0f);

        // Тут матрицы уже две потому что model запускать через цикл!
        ourShader.SetMatrix4("u_View", view);
        ourShader.SetMatrix4("u_Projection", projection);

        // Старый код пульсации цвета (our Color) 
        float TimeValue = Time::GetTime();
        float greenValue = (sin(TimeValue) / 2.0f) + 0.5f;
        ourShader.SetFloat4("ourColor", 0.0f, greenValue, 0.0f, 1.0f);

        // АКТИВИРУЕМ ТЕКСТУРУ ПЕРЕД ОТРИСОВКОЙ!
        wallTexture.Bind(0);
        faceTexture.Bind(1);

        for (unsigned int i = 0; i < 10; i++)
        {
            // Для каждого куба создаём свою собственную матрицу Model
            glm::mat4 model = glm::mat4(1.0f);

            // Сначала смещаем куб в его уникальую точку в мире
            model = glm::translate(model, CubePositions[i]);

            // Каждому кубу задаём свой уникальный угол и ось вращения на основе его индекса 'i'
            float angle = 20.0f * i + 1.0f * Time::GetTime();
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

            ourShader.SetMatrix4("u_Model", model);

            Cube.Draw();

        }
        window.Update();
    }
    return 0;
}
