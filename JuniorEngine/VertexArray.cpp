#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
	: m_VAO(0), m_VBO(0)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
}

VertexArray::~VertexArray()
{
	// Удаляем буфеы из памяти GPU при уничтожении объекта
	if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
}

void VertexArray::SetData(const float* vertices, unsigned int size)
{
	// 1. Включаем VAO
	Bind();

	// 2. Включаем VBO и копируем в него массив вершин
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	
	// 3. Настраиваем указатели (Location = 0, 3 флоата на вершину, шаг 12 байт, без смещения)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Отвязываем всё обратно для безопасности
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	UnBind();
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_VAO);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}


