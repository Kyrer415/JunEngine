#include "Camera.h"
#include "Window.h"
#include "Input.h"

Camera::Camera(glm::vec3 position)
	: m_Position(position),
	m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)), // Смотрим вглубь экрана (минус Z)
	m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),	// Вверх - это ось Y
	m_Speed(5.0f), // Скорость: 5 единиц в секунду
	m_Yaw(-90.0f), // Смотрим строго вперёд
	m_Pitch(0.0f), // голова прямо
	m_Sensitivity(0.1f) // Сенса мышки

{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	// LookAt принимает: 1) Где мы, 2) Куда смотрим (Позция + направлдение), 3) Где вверх
	return glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
}

void Camera::ProcessInput(const Window& window, float deltaTime)
{
	// Вычисляем, насколько свдинемся в зависимости от Delta Time
	float velocity = m_Speed * deltaTime;

	// кнопка W: летим вперёд ( по направлению взгляда)
	if (Input::IsKeyPressed(window, GLFW_KEY_W)) { m_Position += m_Forward * velocity; }
	if (Input::IsKeyPressed(window, GLFW_KEY_S)) { m_Position -= m_Forward * velocity; }

	// Хитрый финт для стрейфа (движения влево\вправо)
	// Вектор "Вправо" вычисляется через векторноле произведение (Cross Product) Вперёд и Вверх!
	glm::vec3 right = glm::normalize(glm::cross(m_Forward, m_Up));

	// Кнопка D: стрейф вправо 
	if (Input::IsKeyPressed(window, GLFW_KEY_D)) { m_Position += right * velocity; }
	// Кнопка A: стрейф влево 
	if (Input::IsKeyPressed(window, GLFW_KEY_A)) {	m_Position -= right * velocity; }
}
void Camera::ProcessMouseMovement(float xOffset, float yOffset)
{
	xOffset *= m_Sensitivity;
	yOffset *= m_Sensitivity;

	m_Yaw += xOffset;
	m_Pitch += yOffset; 

	// ограничеваем наклон головы
	if (m_Pitch > 89.0f) m_Pitch = 89.0f;
	if (m_Pitch < -89.0f) m_Pitch = -89.0f;

	// Пересчитываем векторы на основе новых углов
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	// Страшная Математика триганометрии ( углы эйлера)
	glm::vec3 forward;
	forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	forward.y = sin(glm::radians(m_Pitch));
	forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

	// Полученный вектор делаем единичным (нормализуем)
	m_Forward = glm::normalize(forward);
}