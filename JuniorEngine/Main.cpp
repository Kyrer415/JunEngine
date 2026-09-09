#include <iostream>
#include "Window.h" 
#include "Renderer.h"
#include <glad/glad.h>


// Код Вершинного шейдера
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n" // Принимаем на вход позицию вершины (X, Y, Z)
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" // Передаем позицию дальше в конвейер OpenGL
"}\0";
// Код Фрагментного шейдера
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // Выходной цвет (RGBA)
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n" // Красим пиксель в оранжевый цвет
"}\n\0";

int main()
{
	std::cout << "Engine Startup..." << std::endl;

	// Создаём окно через класс-обертку
	Window window(800, 600, "JuniorEngine via OOP");
	Renderer renderer; // создаём объект нашего рендерера

	// 1. Компиляция ВЕРШИННОГО ШЕЙДЕРА
	// Создаём пустой объект шейдера на видеокарте
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Привязываем наш текст к этому объекту
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	// компилируем
	glCompileShader(vertexShader);

	// Проверка на ошибки компиляции вершинного шейдера
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ОШБИКА: Компиляция Вершинного Шейдера провалена: \n" << infoLog << std::endl;
	}

	// 2. Компиляция ФРАГМЕНТНОГО ШЕЙДЕРА
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// Проверка на ошибки компиляции фрагментного шейддера
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "Ошибка: Компиляция Фрагментного Шейдера провалена:\n" << infoLog << std::endl;
	}

	// 3. Создание ШЕЙДЕРНОЙ ПРОГРАММЫ (ЛИНКОВКА)
	// Объединяем оба шейдера в один рабоичий конвейер
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Проверка на ошибки линковки программы
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Линковка шейдерной программы провалена: \n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

// 4. КООРДИНАТЫ И НАСТРОЙКА БУФЕРОВ (VAO, VBO)
// Массив координат трех вершин треугольника (X, Y, Z)
	float vertices[] = {
		-0.5f, -0.5f,  0.0f,  // Левая нижняя точка
		 0.5f, -0.5f,  0.0f,  // Правая нижняя точка
		 0.0f,  0.5f,  0.0f   // Верхняя точка
	};

	unsigned int VAO, VBO;

	// Генерируем ID для наших объектов на видеокарте
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 1. Привязываем VAO (теперь все настройки буферов будут записываться в него)
	glBindVertexArray(VAO);

	// 2. Привязываем VBO и копируем в него наш массив вершин
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. Объясняем OpenGL, как читать эти данные (настройка указателей)
	// location = 0 (как в шейдере), 3 элемента (X,Y,Z), тип float, без нормализации
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Включаем этот атрибут вершин
	glEnableVertexAttribArray(0);

	// Отвязываем буферы, чтобы случайно их не испортить
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Главный цикл движка
	while (!window.ShouldClose())
	{
		// Очистка экрана -->
		// Теперь очисткой занимается рендерер! Чисто и красиво:
		renderer.Clear(0.1f, 0.1f, 0.14f, 1.0f);

		// АКТИВАЦИЯ ШЕЙДЕРОВ И ОТРИСОВКА
		glUseProgram(shaderProgram);     // 1. Включаем шейдерную программу
		glBindVertexArray(VAO);         // 2. Включаем настройки нашего треугольника
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3. Рисуем: тип примитива, с какой вершины начать, сколько всего вершин


		// обновляем окно (свапаем буферы, опрашваем собиытия)
		window.Update();
	}
	// когда цикл завершится, window уничтожится автоматически, так как сработает деструктор ~Window() и сам закроет GLFW


	return 0;
}