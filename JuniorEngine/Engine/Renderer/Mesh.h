#pragma once

#include "Renderer\VertexArray.h"	
#include "Renderer\Texture.h"	
#include <vector>
#include <glm/glm.hpp>

// Структура, описывающая одну вершину в памяти
// 3 флоата на позицию + 2 на текстуру + 3 на нормаль = 8 флоатов (ровно 32 байта)
struct Vertex
{
	glm::vec3 Position; // Смещение от начала: 0 байт
	glm::vec2 TexCoords; // Смещение от начала: 12 байт (3 * 4)
	glm::vec3 Normal; // Смещение от начала: 20 байт (12 + 2 * 4)
};

class Mesh
{
public:
	// Конструктор
	Mesh();

	// Конструктор, который сраззу принимает массив вершин и его размер в байтах
	Mesh(const Vertex* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iSize);
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures);

	// Метод для загрузки или обновления данных
	void SetData(const Vertex* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iSize);

	// Метод дя отрисовки меша
	void Draw() const;

private:
	VertexArray m_VAO;
	unsigned int m_IndexCount; // Переменная, которая запомнит, сколько у нас соединений

	// вектор текстур, принадлежащих конкретно этому мешу!
	std::vector<Texture> m_Textures;
};