#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <string>

// forward decalration на GLFW чтбы не засорять файыл проекта.
struct GLFWwindow; 

class Window
{
public:
	// конструктор: принимает размеры и заголовок окна
	Window(int width, int height, const std::string& title);

	// деструктор: очистит ресурсы при закрытии приложения
	~Window();

	// Проверяем, не нажал ли пользователь на "крестик" окна
	bool ShouldClose() const;

	// Принудительное закрытие
	void Close();

	// Обновляем окно: меняет буферы и опрашивает события (мышь\клавиатура)
	void Update();

	// Получить сырой указатель на окно GLFW (нужно для подсистемы ввода)
	struct GLFWwindow* GetNativeWindow() const { return m_Window; }

	void DisableCursor() const;

	// Получить Ширину и высоту 
	int GetWidth() const { return m_Width; }
	int GetHeight() const { return m_Height; }

private:
	// Внутренний метод инициилизации GLFW и создания окна
	bool Init();
	// метод для корректного закрытия окна
	void Shutdown();

private:
	int m_Width;
	int m_Height;
	std::string m_Title;

	// Указатель на реальное окно GLFW
	GLFWwindow* m_Window;

};