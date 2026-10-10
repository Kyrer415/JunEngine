#include "ImGuiManager.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/glm.hpp>
#include "Core/Transform.h" // чтобы менеджер видел поля Position, Rotation, Scale

void ImGuiManager::RenderUI(SceneSettings& settings, Transform& tableTransform)
{
    ImGui::Begin("Engine Control Panel");
    ImGui::Text("JuniorEngine Debag Menu");
    ImGui::Separator();

    // Управление трансформацией стола
    ImGui::Text("Table Object Transform:");

    // Достаём ссылку на наш трансформ один раз, чтобы не спамить вызовом метода
    //Transform& tableTransform = tableModel.GetTransform(); - теперь не катит мы сверху падаём ссылку на параметры стола :3

    ImGui::SliderFloat3("Table Position", &tableTransform.Position.x, -10.0f, 10.0f);
    ImGui::SliderFloat3("Table Rotation", &tableTransform.Rotation.x, 0.0f, 360.0f);
    ImGui::SliderFloat3("Table Scale", &tableTransform.Scale.x, 0.1f, 5.0f);
    ImGui::Separator();

    // 1. Управление материалом кубов
    ImGui::Text("Material Settings:");
    ImGui::SliderFloat("Shininess", &settings.Shininess, 1.0f, 256.0f);

    ImGui::Text("Transform Settings:");
    ImGui::Separator();

    ImGui::SliderFloat("Scale Cube", &settings.CubeScale, -5.0f, 20.0f);

    ImGui::Separator();

    // Управление летающей лампачкеой
    ImGui::Text("Light Source Settings:");
    ImGui::SliderFloat("Orbit Radius", &settings.lightRadius, 0.5f, 10.0); // крутим радиус от 0.5 до 10 
    ImGui::SliderFloat("Orbit Height", &settings.lightHeight, -5.0f, 5.0f); // крутим высоту от -5 до 5
    ImGui::SliderFloat("Orbit Speed", &settings.lightOrbitSpeed, 0.0f, 5.0f); // Скорость от 0(стоп) до 5


    // Тот самый ColorPicker! Принимает имя и указатель на массив из 3 флоатов (RGB)
    ImGui::ColorEdit3("Light Color", &settings.lightColorIntensity.x);

    ImGui::Checkbox("Enable Disco Mode", &settings.isDiscoMode);


    ImGui::End();
}

void ImGuiManager::BeginFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::EndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Выгружаем ресурсы интерфейса из пямяти
ImGuiManager::~ImGuiManager()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

ImGuiManager::ImGuiManager(GLFWwindow* nativeWindow)
{
    // 1. Создаём контекст Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Включаем тёмную стильную тему интерфейса 
    ImGui::StyleColorsDark();

    // 2. Инициилизируем мосты (платформы и реендер)
    ImGui_ImplGlfw_InitForOpenGL(nativeWindow, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}
