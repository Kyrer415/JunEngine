#include "Renderer\VertexArray.h"
#include "Mesh.h"
#include <glad/glad.h>
#include <cstddef>

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

VertexArray::VertexArray(VertexArray&& o) noexcept
	: m_VAO(o.m_VAO), m_VBO(o.m_VBO), m_EBO(o.m_EBO)
{
	o.m_VAO = o.m_VBO = o.m_EBO = 0;
}

VertexArray& VertexArray::operator=(VertexArray&& o) noexcept
{
	if (this != &o)
	{
		if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
		if (m_VBO) glDeleteBuffers(1, &m_VBO);
		if (m_EBO) glDeleteBuffers(1, &m_EBO);
		m_VAO = o.m_VAO; m_VBO = o.m_VBO; m_EBO = o.m_EBO;
		o.m_VAO = o.m_VBO = o.m_EBO = 0;
	}
	return *this;
}

void VertexArray::SetData(const Vertex* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iSize)
{
	// 1. Включаем VAO
	Bind();

	// 2. Включаем VBO и копируем в него массив вершин
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, vCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// 2. Загружаем индексы в EBO для индексов используется специлальный тип GL_ELEMENT_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);
	
	// 3. НАСТРОЙКА АТРИБУТА 0: Позиция(X, Y, Z). Шаг - размер структуры Vertex (32 байта)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
	glEnableVertexAttribArray(0);

	// 4. НАСТРЙОКА АТРИБУТТА 1: Текстурные координаты (U, V). 
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(1);

	// 5. НОВЫЙ АТРБУТ 2: Вектор нормали(nX, nY, nZ). Шаг 32 байта.
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(2);

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


