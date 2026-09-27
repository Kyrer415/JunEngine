#pragma once

// forward declarationm, чтобы не тазить glfw в хедер
class Window;

class Input
{
public:
	// Проверяем, зажата ли конкретная клавиша в указанном ООП-окне
	static bool IsKeyPressed(const Window& window, int key);
};