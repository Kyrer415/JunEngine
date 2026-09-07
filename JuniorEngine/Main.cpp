#include <iostream>
#include <glad/glad.h>
#include <GLFW//glfw3.h>



int main()
{
	std::cout << "Engine Startup..." << std::endl;

	if (!glfwInit()) 
	{
		std::cerr << "Не удалось инициилизировать GLFW" << std::endl;
		return -1;
	}

	// Настройка версии OpenGL (3.3 Core)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Создание окна
	GLFWwindow* window = glfwCreateWindow(800, 600, "JuniorEngine", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Не удалось создать окно GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Инициилизация GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << " Не удалось инициилизировать GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// MainLoop JuniorEngine
	while (!glfwWindowShouldClose(window))
	{
		// Очистка экрана тёмно-серым светом
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Смена буферов и опрос событий
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}