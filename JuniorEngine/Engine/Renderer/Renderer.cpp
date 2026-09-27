#include "Renderer.h"
#include <glad/glad.h>

void Renderer::Clear(float r, float g, float b, float a) const
{
	// зададим цвет очистки
	glClearColor(r, g, b, a);
	// Очищаем буфер цвета (заливаем экран этим цветом)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}