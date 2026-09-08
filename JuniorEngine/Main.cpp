#include <iostream>
#include <glad/glad.h>
#include "Window.h" 



int main()
{
	std::cout << "Engine Startup..." << std::endl;

	// Создаём окно через класс-обертку
	Window window(800, 600, "JuniorEngine via OOP");

	// Главный цикл движка
	while (!window.ShouldClose())
	{
		// Очистка экрана (пока оставим эту функцию OpenGL здесь
		// Но в будущем её перенесу в класс Renderer!
		// Доступны функции gl, так как glad.c  подключен к проекту
		glClearColor(0.1f, 0.1f, 0.14f, 1.0f); // цвет чуть темнее
		glClear(GL_COLOR_BUFFER_BIT);

		// обновляем окно (свапаем буферы, опрашваем собиытия)
		window.Update();
	}
	// когда цикл завершится, window уничтожится автоматически, так как сработает деструктор ~Window() и сам закроет GLFW


	return 0;
}