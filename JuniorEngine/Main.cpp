#include <iostream> 

#include "Renderer\Texture.h"
#include "Core/Application.h"


int main()
{
    setlocale(LC_ALL, "Russian");

    std::cout << "Engine Startup...\n";

    Texture::SetFlipVerticallyOnLoad(1);
    Application app;
    app.Run();

    return 0;
}
