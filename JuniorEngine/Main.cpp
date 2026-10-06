#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream> 
#include <cmath>

#include "Window\Window.h"
#include "Renderer\Shader.h" 
#include "Renderer\Renderer.h"
#include "Renderer\Mesh.h"
#include "Renderer/Model.h"
#include "Renderer\Texture.h"
#include "Renderer\Camera.h" // подключили камеру
#include "Core\CoreTime.h"
#include "Core\Input.h"

int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Engine Startup...\n";
    
    // Создаем подсистемы движка
    Window window(1600, 1200, "JuniorEngine via OOP");

    // 1. Создаём контекст Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Включаем тёмную стильную тему интерфейса 
    ImGui::StyleColorsDark();

    // 2. Инициилизируем мосты (платформы и реендер)
    ImGui_ImplGlfw_InitForOpenGL(window.GetNativeWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    Renderer renderer;
    Shader ourShader("basic.vert", "basic.frag");
    Shader lightShader("basic.vert", "light.frag"); // шейдер для поинтлайта

    // Массив вершин куба: Координаты (X,Y,Z) + Текстурные координаты (U,V)
        // Массив вершин куба: Позиция (X,Y,Z) + Текстура (U,V) + Нормали (nX,nY,nZ)
    Vertex vertices[] = {
        // Позиция                     // Текстура    // Нормаль
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
        { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
        { glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
        { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
        { glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
        { glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
        { glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
        { glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
        { glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
        { glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
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

    unsigned int vertexCount = sizeof(vertices) / sizeof(Vertex);
    Mesh Cube(vertices, vertexCount, indices, sizeof(indices));

    // Включаем ТЕСТ глубины для настоящего 3d!
    renderer.EnableZ();

    // Загружаем наш столик через Assimp
    Model tableModel("models/Table.obj");

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
    
    glm::vec3 lightpos(1.2f, 1.0f, 2.0f);

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
    bool isUIFocused = false; // По умолчанию мы в режиме полёта(курсор зафиксирован и спрятан)
    bool altKeyReleased = true; // Предохранитель от зажимания

    // Наш отдел настроек для ImGui
    float testShininess = 32.0f; // Дефолтная глянцевость
    float lightRadius = 2.5f; // Радиус орбиты лампочки
    float lightOrbitSpeed = 1.0f; // Скорость вращения
    float lightheight = 2.0f;
    float lightColorIntensity[3] = { 1.0f, 1.0f, 1.0f }; // Цвет лампы (RGB массив для ImGui)

    float CubeScale = 1;

    bool isDiscoMode = false; // Наш диско-режим

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

        // Переключаем режим на кнопку LEFT ALT ( код 342 в GLFW / Input), проверяя что с пролшлого нажатия прошло больше 0.2 сек
        if (Input::IsKeyPressed(window, 342))
        {
            if (altKeyReleased)
            {
                isUIFocused = !isUIFocused;


                if (isUIFocused)
                    window.EnableCursor(); // освобождаем мышь для ImGui
                else
                    window.DisableCursor();

                // небольшая задержка, чтобы кнопка не спамила переключением за один кадр
                firstMouse = true; // Сбрасываем скачок камеры при возврате

                altKeyReleased = false; // блокируем повторные нажаития
            }

        }
        else
        {
            altKeyReleased = true;
        }

        // Обрабатываем движение камеры только если мышь не занята интерфейсом!
        if (!isUIFocused)
        {
            double mouseX, mouseY;
            glfwGetCursorPos(window.GetNativeWindow(), &mouseX, &mouseY);

            if (firstMouse)
            {
                lastX = (float)mouseX;
                lastY = (float)mouseY;
                firstMouse = false;
            }
            // Cчитаем смещение мыши между текущим и прошлым кадром
            float xOffset = (float)mouseX - lastX;
            // Инвертируем Y, так как в GLFW координаты экрна идут сверху вниз, а в 3D снизу вверх
            float yOffset = lastY - (float)mouseY;
            // Запоминаем текущие координаты как "Прошлые" для след. кадра
            lastX = (float)mouseX;
            lastY = (float)mouseY;

            // Передаём дельту перемещения в класс камеры
            camera.ProcessMouseMovement(xOffset, yOffset);
        }

        // Старт кадра ImGui 
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Создаём наше всплывающее дебаг-окно!
        ImGui::Begin("Engine Control Panel");
        ImGui::Text("JuniorEngine Debag Menu");
        ImGui::Separator();

        // 1. Управление материалом кубов
        ImGui::Text("Material Settings:");
        ImGui::SliderFloat("Shininess", &testShininess, 1.0f, 256.0f);

        ImGui::Text("Transform Settings:");
        ImGui::Separator();

        ImGui::SliderFloat("Scale Cube", &CubeScale, 20.0f, -5.0f);
        
        ImGui::Separator();

        // Управление летающей лампачкеой
        ImGui::Text("Light Source Settings:");
        ImGui::SliderFloat("Orbit Radius", &lightRadius, 0.5f, 10.0); // крутим радиус от 0.5 до 10 
        ImGui::SliderFloat("Orbit Height", &lightheight, -5.0f, 5.0f); // крутим высоту от -5 до 5
        ImGui::SliderFloat("Orbit Speed", &lightOrbitSpeed, 0.0f, 5.0f); // Скорость от 0(стоп) до 5
  

        // Тот самый ColorPicker! Принимает имя и указатель на массив из 3 флоатов (RGB)
        ImGui::ColorEdit3("Light Color", lightColorIntensity);
        
        ImGui::Checkbox("Enable Disco Mode", &isDiscoMode);


        ImGui::End();


        renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

        ourShader.Use();

        // Считаем новые координаты лампы по круговой орбите
        // lightColorIntensity и прочие наши параметры передаём от ImGui! :3
        float lightX = sin(Time::GetTime() * lightOrbitSpeed) * lightRadius;
        float lightZ = cos(Time::GetTime() * lightOrbitSpeed) * lightRadius;
        float lightY = sin(Time::GetTime() * 2.0f) * 0.5f + lightheight;

        ourShader.Use();
        ourShader.SetFloat3("u_LightPos", lightX, lightY, lightZ);

        if (isDiscoMode)
        {
            // Плавно меняем RGB от времени.
            // Синус бывает от -1 до 1, поэтому делаем * 0.5 + 0.5, чтобы получить чистые цвета от 0 до 1!
            float r = sin(Time::GetTime() * 1.5f) * 0.5f + 0.5f;
            float g = sin(Time::GetTime() * 2.0f) * 0.5f + 0.5f;
            float b = sin(Time::GetTime() * 1.0f) * 0.5f + 0.5f;

            ourShader.SetFloat3("u_LightColor", r, g, b); // наш диско цвет

        }
        else
        {
            ourShader.SetFloat3("u_LightColor", lightColorIntensity[0], lightColorIntensity[1], lightColorIntensity[2]); // наш белый цвет :3
        }


        ourShader.SetFloat3("u_ViewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

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
        ourShader.SetFloat4("ourColor", 1.0f, 1.0, 1.0f, 1.0f);

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

            model = glm::scale(model, glm::vec3(1.0f, CubeScale, 1.0f));

            ourShader.SetMatrix4("u_Model", model);

            if (i % 2 == 0)
            {
                ourShader.SetFloat("material.ambient", 0.1f);
                ourShader.SetFloat("material.diffuse", 1.0f);
                ourShader.SetFloat("material.specular", 1.0f);
                ourShader.SetFloat("material.shininess", testShininess); // слайдер управляет этим!
            }
            else
            {
                ourShader.SetFloat("material.ambient", 0.01f);
                ourShader.SetFloat("material.diffuse", 0.8f);
                ourShader.SetFloat("material.specular", 0.0f);
                ourShader.SetFloat("material.shininess", 1.0f);
            }

            Cube.Draw();

        }

        // Отрисовка 3д модели
        ourShader.Use();

        // Строим матрицу трансформации для стола
        glm::mat4 tableTransform = glm::mat4(1.0f);

        // Свдигаем его чуть чуть вглубб экрана и пониже, чтобы не перекрывал кубы
        tableTransform = glm::translate(tableTransform, glm::vec3(0.0f, 0.0f, 0.0f));

        // Крутим стол вокруг своей оси от времени
        tableTransform = glm::rotate(tableTransform, (float)Time::GetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

        // Ставим масштаб(размер)
        tableTransform = glm::scale(tableTransform, glm::vec3(5.0f));

        // Закидываем матрицу модели в шейдер
        ourShader.SetMatrix4("u_Model", tableTransform);

        // Настраиваем дефолтный материал для стола (сделаем его матовым)
        ourShader.SetFloat("material.ambient", 0.1f);
        ourShader.SetFloat("material.diffuse", 0.8f);
        ourShader.SetFloat("material.specular", 0.0f);
        ourShader.SetFloat("material.shininess", 1.0f);

        // Вызваем Draw классса Model, передавая туда шейдер!
        wallTexture.Bind(0);
        faceTexture.Bind(1);
        tableModel.Draw(ourShader);

        // Отрисвка кубика для лампы -->
        lightShader.Use();

        // Передаём в шейдер лампы те же самые общие матрицы View и Projection
        lightShader.SetMatrix4("u_View", view);
        lightShader.SetMatrix4("u_Projection", projection);


        // Строим матрицу Model для лампы строго в её летающих координатах!
        glm::mat4 lightModel = glm::mat4(1.0f);


        lightModel = glm::translate(lightModel, glm::vec3(lightX, lightY, lightZ));

        // Масштабируем кубик лампы, делая его маленьким ( в 5 раз меньше обычного)
        lightModel = glm::scale(lightModel, glm::vec3(0.1f));

        lightShader.SetMatrix4("u_Model", lightModel);

        Cube.Draw();

        // Финальный рендер кадра ImGui на экран
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.Update();
    }

    // выгружаем ресурсы интерфейса из памяти
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    return 0;
}
