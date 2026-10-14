#include <iostream> 
#include <exception>

#include "Renderer\Texture.h"
#include "Core/Application.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	Texture::SetFlipVerticallyOnLoad(true);

	std::cout << "Engine Startup...\n";

	try
	{
		Application app;
		app.Run();
	}
	catch (const std::exception& e)
	{
		std::cerr << "Критическая ошибка: " << e.what() << "\n";
		return 1; // ненулевой код значит что с ошибкой
	}

	return 0;
}
