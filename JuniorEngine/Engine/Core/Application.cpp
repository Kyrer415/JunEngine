#include "Application.h"
#include "Core/Transform.h"

Application::Application()
    : m_Window(1600, 1200, "JuniorEngine via OOP"),
    m_Renderer(),
    m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)), // начальня позиция камеры
    m_OurShader("basic.vert", "basic.frag"),
    m_LightShader("basic.vert", "light.frag"),
    m_TableModel(Model::LoadModelFromDisk( "models/Table.obj")),
	m_ImGui(m_Window.GetNativeWindow()), // Передаем нативное окно в конструктор ImGuiManager!
	m_Settings(SceneSettings()),
    m_CubeMesh(Mesh::CreateCube()),
    m_WallTexture(Texture::LoadImageFromDisk("openGL_logo.png")),
    m_FaceTexture(Texture::LoadImageFromDisk("UnrealEngine_Logo.png")),
    m_TableMaterial{ 0.1f, 0.8f, 0.0f, 1.0f }

{
	// пока пусто
}

Application::~Application()
{

}

void Application::Run()
{
    m_Window.DisableCursor();

    m_OurShader.Use();
    m_OurShader.SetInt("texture1", 0); // Меш привяжет диффузную карту в слот 0
    m_OurShader.SetInt("texture2", 1);

    while (!m_Window.ShouldClose())
    {
        // В самом начале кадра: обновляем Delta Time!
        Time::Update();

        m_Renderer.EnableZ();

        // Обработка ввода: если нажат ESCAPE (код 256), закрываем движок!
        if (Input::IsKeyPressed(m_Window, GLFW_KEY_ESCAPE))
        {
            m_Window.Close();
        }

        // Переключаем режим на кнопку LEFT ALT , проверяя что с пролшлого нажатия прошло больше 0.2 сек
        if (Input::IsKeyPressed(m_Window, GLFW_KEY_LEFT_ALT))
        {
            if (m_Settings.altKeyReleased)
            {
                m_Settings.isUIFocused = !m_Settings.isUIFocused;


                if (m_Settings.isUIFocused)
                    m_Window.EnableCursor(); // освобождаем мышь для ImGui
                else
                    m_Window.DisableCursor();

                m_Camera.ResetFirstMouse(); // сбрасываем чтобы камера не прыгнула при возврате в режим полёта

                m_Settings.altKeyReleased = false; // блокируем повторные нажаития
            }
        }
        else
        {
            m_Settings.altKeyReleased = true;
        }

        // Обрабатываем движение камеры только если мышь не занята интерфейсом!
        if (!m_Settings.isUIFocused)
        {

            m_Camera.Update(m_Window, Time::GetDeltaTime());

        }

        m_ImGui.BeginFrame();
        m_ImGui.RenderUI(m_Settings, m_TableModel.GetTransform());

        m_Renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);
        m_OurShader.Use();

        // Считаем новые координаты лампы по круговой орбите
    // lightColorIntensity и прочие наши параметры передаём от ImGui! :3
        float lightX = sin(Time::GetTime() * m_Settings.lightOrbitSpeed) * m_Settings.lightRadius;
        float lightZ = cos(Time::GetTime() * m_Settings.lightOrbitSpeed) * m_Settings.lightRadius;
        float lightY = sin(Time::GetTime() * 2.0f) * 0.5f + m_Settings.lightHeight;

        m_OurShader.SetFloat3("u_LightPos", lightX, lightY, lightZ);


        if (m_Settings.isDiscoMode)
        {
            // Плавно меняем RGB от времени.
            // Синус бывает от -1 до 1, поэтому делаем * 0.5 + 0.5, чтобы получить чистые цвета от 0 до 1!
            float r = sin(Time::GetTime() * 1.5f) * 0.5f + 0.5f;
            float g = sin(Time::GetTime() * 2.0f) * 0.5f + 0.5f;
            float b = sin(Time::GetTime() * 1.0f) * 0.5f + 0.5f;

            m_OurShader.SetFloat3("u_LightColor", r, g, b); // наш диско цвет

        }
        else
        {
            m_OurShader.SetFloat3("u_LightColor", m_Settings.lightColorIntensity[0], m_Settings.lightColorIntensity[1], m_Settings.lightColorIntensity[2]); // наш белый цвет :3
        }


        m_OurShader.SetFloat3("u_ViewPos", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);

        // 3. Матрица VIEW (Наша виртуальная камера)
        glm::mat4 view = m_Camera.GetViewMatrix();

        // 4. МАТРИЦА PROJECTION ( Перспектива)
        // Параметры: угол обзора 45 градусов, соотношение сторон экрана , ближняя плоскость, дальняя плоскость
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window.GetWidth() / (float)m_Window.GetHeight(), 0.1f, 100.0f);

        // Тут матрицы уже две потому что model запускать через цикл!
        m_OurShader.SetMatrix4("u_View", view);
        m_OurShader.SetMatrix4("u_Projection", projection);

        m_WallTexture.Bind(0);
        m_FaceTexture.Bind(1);

        for (size_t i = 0; i < m_Scene.GetCubes().size(); i++)
        {
            const SceneObject& obj = m_Scene.GetCubes()[i];

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, obj.transform.Position);

            float angle = 20.0f * i + 1.0f * Time::GetTime();
            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            model = glm::scale(model, glm::vec3(1.0f, m_Settings.CubeScale, 1.0f));

            m_OurShader.SetMatrix4("u_Model", model);

            Material mat = obj.material;
            if (mat.specular > 0.0f)
                mat.shininess = m_Settings.Shininess;
            m_OurShader.SetMaterial("material", mat);

            m_CubeMesh.Draw();
        }

        // Отрисовка 3д модели
        m_OurShader.Use();

        glm::mat4 tableModelMatrix = m_TableModel.GetTransform().GetModelMatrix();

        // Закидываем матрицу модели в шейдер
        m_OurShader.SetMatrix4("u_Model", tableModelMatrix);

        // Настраиваем дефолтный материал для стола (сделаем его матовым)
        m_OurShader.SetMaterial("material", m_TableMaterial);

        // Вызваем Draw классса Model, уже не передавая туда шейдер!
        m_TableModel.Draw();

        // Отрисвка кубика для лампы -->
        m_LightShader.Use();

        // Передаём в шейдер лампы те же самые общие матрицы View и Projection
        m_LightShader.SetMatrix4("u_View", view);
        m_LightShader.SetMatrix4("u_Projection", projection);


        // Строим матрицу Model для лампы строго в её летающих координатах!
        glm::mat4 lightModel = glm::mat4(1.0f);


        lightModel = glm::translate(lightModel, glm::vec3(lightX, lightY, lightZ));

        // Масштабируем кубик лампы, делая его маленьким ( в 5 раз меньше обычного)
        lightModel = glm::scale(lightModel, glm::vec3(0.1f));

        m_LightShader.SetMatrix4("u_Model", lightModel);

        m_CubeMesh.Draw();

        // Финальный рендер кадра ImGui на экран
        m_ImGui.EndFrame();

        m_Window.Update();
    }
}