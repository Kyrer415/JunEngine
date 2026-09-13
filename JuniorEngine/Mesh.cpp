#include "Mesh.h"
#include <glad/glad.h>

Mesh::Mesh()
	: m_VertexCount(0)
{
}

Mesh::Mesh(const float* vertices, unsigned int size)
	: m_VertexCount(0)
{
	SetData(vertices, size);
}

void Mesh::SetData(const float* vertices, unsigned int size)
{
	// Загружаем данные в наш VertexArray
	m_VAO.SetData(vertices, size);

	// Вычисляем кол-во вершин. size - размер всего массива в байтах. Каждая вершина состоит из 3 чисел f(XYZ). Значит размер одной вершины в байтах равен 3 * sizeof(float) (то есть 12 байт).
	m_VertexCount = size / (3 * sizeof(float));
}

void Mesh::Draw() const
{
	// Если вершин нет, то и рисовать нечего
	if (m_VertexCount == 0) return;

	// 1. Активируем буферы этого меша
	m_VAO.Bind();

	// 2. Отдаём команду видеокарте на отрисовку!
	glDrawArrays(GL_TRIANGLES, 0, m_VertexCount);

	// 3. Отвязываем обратно для безопасности
	m_VAO.UnBind();
}