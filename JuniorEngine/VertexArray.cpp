#include "VertexArray.h"
#include <glad/glad.h>

VertexArray::VertexArray()
	: m_VAO(0), m_VBO(0), m_EBO(0)
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);
}

VertexArray::~VertexArray()
{
	// Удаляем буфеы из памяти GPU при уничтожении объекта
	if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
	if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
	if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);
}

void VertexArray::SetData(const float* vertices, unsigned int vSize, const unsigned int* indices, unsigned int iSize)
{
	// 1. Включаем VAO
	Bind();

	// 2. Включаем VBO и копируем в него массив вершин
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vSize, vertices, GL_STATIC_DRAW);

	// 2. Загружаем индексы в EBO для индексов используется специлальный тип GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
	
	// 3. Настраиваем указатели (Location = 0, 3 флоата на вершину, шаг 12 байт, без смещения)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Отвязываем только VBO! 
	// ВАЖНО:glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) писать НЕЛЬЗЯ, иначе VAO забудет индексы!
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


