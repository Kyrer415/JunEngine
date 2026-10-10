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
#include "Renderer\Model.h"
#include "Renderer\Texture.h"
#include "Renderer\Camera.h" 
#include "Core\CoreTime.h"
#include "Core\Input.h"
#include "Core/Application.h"


int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Engine Startup...\n";

    Application app;
    app.Run();

    return 0;
}
