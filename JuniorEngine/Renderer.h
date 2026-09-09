#pragma once

class Renderer
{
public:
	// Метод для очистки экрана заданным цветом (RGBA)
	void Clear(float r, float g, float b, float a = 1.0f) const;
};