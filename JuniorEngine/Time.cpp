#include "Time.h"
#include <GLFW/glfw3.h>

float Time::m_DeltaTime = 0.0f;
float Time::m_LastFrameTime = 0.0f;

float Time::GetTime()
{
	// Забираем время у GLFW (он возвращает double, мы приводим его к float))
	return static_cast<float>(glfwGetTime());
}

void Time::Update()
{
	// 1. Получаем текущее время в секундах
	float currentFramTime = GetTime();

	// 2. Вычисляем разницу между текущим и прошлым кадром
	m_DeltaTime = currentFramTime - m_LastFrameTime;

	// 3. Запоминаем текущее время как "Прошлое" для следующего кадра
	m_LastFrameTime = currentFramTime;
}