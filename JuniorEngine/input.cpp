#include "Input.h"
#include "Window.h"
#include <GLFW/glfw3.h>

bool Input::IsKeyPressed(const Window& window, int key)
{
    // Получаем сырой указатель на окно GLFW из нашей ООП-обертки Window
    GLFWwindow* nativeWindow = window.GetNativeWindow();

    // Спрашиваем у GLFW статус кнопки (GLFW_PRESS означает, что кнопка зажата)
    int state = glfwGetKey(nativeWindow, key);
    return state == GLFW_PRESS;
}
