#include "Window\Window.h"
#include <iostream>

// GLAD строго перед GLFW!
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Выделяем память под статический счетчик окон
int Window::s_WindowCount = 0;


// конструктор: сохраняем настройки и запускаем инициализацию
Window::Window(int width, int height, const std::string& title) : m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr)
{
	Init();
}

// Деструктор: сработаем сам, когда окно закроется 
Window::~Window()
{
	Shutdown(); // тут есть проверка на валидность if (m_Window) так что всё гуд
}

Window::Window(Window&& o) noexcept
	: m_Width(std::move(o.m_Width)), m_Height(std::move(o.m_Height)), m_Title(std::move(o.m_Title)), m_Window(o.m_Window)
{
	o.m_Window = 0;
}

Window& Window::operator=(Window&& o) noexcept
{
	if (this != &o)
	{
		if (m_Window) Shutdown();
		m_Window = o.m_Window;
		m_Width = o.m_Width;
		m_Height = o.m_Height;
		m_Title = std::move(o.m_Title);
		o.m_Window = 0;
	}
	return *this;
}

bool Window::Init()
{
	if (s_WindowCount == 0)
	{
		// 1. Инициализация GLFW
		if (!glfwInit())
		{
			std::cerr << "Не удалось инициилизировать GLFW";
			return false;
		}
	}
	s_WindowCount++;

	// Настройки OpenGL 3.3 Core
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	// 2. Слздание окна
	m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
	if (!m_Window)
	{
		std::cerr << "Не удалось создать окно GLFW!" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_Window);

	// 3. Инициилизиация GLAD 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Не удалось инициилизировать GLAD!" << std::endl;
		return false;
	}

	glViewport(0, 0, m_Width, m_Height);
	return true;
}

bool Window::ShouldClose() const
{
	return glfwWindowShouldClose(m_Window);
}

void Window::Update()
{
	glfwSwapBuffers(m_Window);
	glfwPollEvents();
}

void Window::Shutdown()
{
	if (m_Window)
	{
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
		s_WindowCount--;
		if (s_WindowCount == 0)
		{
			glfwTerminate();
		}
	}
}
void Window::Close()
{
	glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void Window::DisableCursor() const
{
	// Приказываем GLFW скрыть курсор и запереть его внутри окна
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::EnableCursor() const
{
	// Приказываем GLFW показать курсор 
	glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}