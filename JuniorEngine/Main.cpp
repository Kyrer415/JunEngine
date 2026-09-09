#include <iostream>
#include "Window.h" 
#include "Renderer.h"



int main()
{
	std::cout << "Engine Startup..." << std::endl;

	// Создаём окно через класс-обертку
	Window window(800, 600, "JuniorEngine via OOP");
	Renderer renderer; // создаём объект нашего рендерера


	// Главный цикл движка
	while (!window.ShouldClose())
	{
		// Очистка экрана -->
		// Теперь очисткой занимается рендерер! Чисто и красиво:
		renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

		// обновляем окно (свапаем буферы, опрашваем собиытия)
		window.Update();
	}
	// когда цикл завершится, window уничтожится автоматически, так как сработает деструктор ~Window() и сам закроет GLFW


	return 0;
}