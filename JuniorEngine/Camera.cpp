#include "Camera.h"
#include "Window.h"
#include "Input.h"

Camera::Camera(glm::vec3 position)
	: m_Position(position),
	m_Forward(glm::vec3(0.0f, 0.0f, -1.0f)), // Смотрим вглубь экрана (минус Z)
	m_Up(glm::vec3(0.0f, 1.0f, 0.0f)),	// Вверх - это ось Y
	m_Speed(5.0f) // Скорость: 5 единиц в секунду
{
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
	if (Input::IsKeyPressed(window, 87)) // 87 - код клавиши W в GLFW	
	{
		m_Position += m_Forward * velocity;
	}
	if (Input::IsKeyPressed(window, 83)) // 83 - код клавиши S в GLFW	
	{
		m_Position -= m_Forward * velocity;
	}

	// Хитрый финт для стрейфа (движения влево\вправо)
	// Вектор "Вправо" вычисляется через векторноле произведение (Cross Product) Вперёд и Вверх!
	glm::vec3 right = glm::normalize(glm::cross(m_Forward, m_Up));

	// Кнопка D: стрейф вправо
	if (Input::IsKeyPressed(window, 68)) // 68 - клавиша D
	{
		m_Position += right * velocity;
	}
	// Кнопка A: стрейф влево
	if (Input::IsKeyPressed(window, 65)) // 65 - клавиша A
	{
		m_Position -= right * velocity;
	}
}
