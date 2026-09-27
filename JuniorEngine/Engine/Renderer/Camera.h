#pragma once

#include <glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Window; // Forward Decalration

class Camera
{
public:
	// Конструктор принимает начальную позицию камеры
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f));

	// Геттер матрицы вида (view Matrix) для шейдера
	glm::mat4 GetViewMatrix() const;

	// Метод для перемещения камеры с клавиатуры
	void ProcessInput(const Window& window, float deltaTime);

	// Метод для мышки! принимает смещение мыши по x и y
	void ProcessMouseMovement(float xOffset, float yOffset);

private:
	// Внутренний метод, который пересчитывает вектор m_Forward из углов Yaw\Pitch
	void UpdateCameraVectors();

	glm::vec3 m_Position; // Позиция камеры в 3D мире
	glm::vec3 m_Forward; // Вектор направления вперёд
	glm::vec3 m_Up; // Вектор направления вверх
	
	float m_Speed; // Скорость перемещения камеры

	// новые переменные для обзора мышью
	float m_Yaw; // Угол наклона влево-вправо
	float m_Pitch; // Угол наколна вверх-вниз
	float m_Sensitivity; // Чуствительность мыши
};