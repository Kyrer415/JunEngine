#include "Time.h"
#include <GLFW/glfw3.h>

float Time::GetTime()
{
	// Забираем время у GLFW (он возвращает double, мы приводим его к float)
	return static_cast<float>(glfwGetTime());
}