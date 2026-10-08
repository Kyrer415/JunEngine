#include "Renderer\Mesh.h"
#include <glad/glad.h>

Mesh::Mesh()
	: m_IndexCount(0)
{
}

Mesh::Mesh(const Vertex* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iSize)
	: m_IndexCount(0)
{
	SetData(vertices, vCount, indices, iSize);
}

// Новый конструктор с текстурами!
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, std::vector<Texture> textures)
{
	m_IndexCount = indices.size();
	m_Textures = std::move(textures); // копируем текстуры внутрь меша СТООООП не копируем, а перемещаем!!!!

	// Умножаем кол-во элементов на размер одного unsigned int (4 байта)
	unsigned int indiciesSizeInBytes = indices.size() * sizeof(unsigned int);
	m_VAO.SetData(vertices.data(), (unsigned int)vertices.size(), indices.data(), indiciesSizeInBytes);
}

void Mesh::SetData(const Vertex* vertices, unsigned int vCount, const unsigned int* indices, unsigned int iSize)
{
	// Загружаем данные в наш VertexArray
	m_VAO.SetData(vertices, vCount, indices, iSize);

	// Больше вершины не считаем, просто знаем кол-во индексов для отрисовки, iSize - это размер массива индексов в байтах, иднекс это тип uns int - 4 байта, просто делим общий размер в байтах на размер одного uns int
	m_IndexCount = iSize / sizeof(unsigned int);
}

void Mesh::Draw() const
{
	// Если вершин нет, то и рисовать нечего
	if (m_IndexCount == 0) return;

	for (unsigned int i = 0; i < m_Textures.size(); i++)
	{
		// Передаём индекс слота (0,1,2...), наш класс текстуры вызовёт glActiveTexture внутри!
		m_Textures[i].Bind(i);
	}

	// 1. Активируем буферы этого меша
	m_VAO.Bind();

	// 2. ВАЖНО: Вызываем glDrawElements вместо glDrawArrays!
	// Параметры: тип примитива, количество индексов, тип данных индексов, смещение
	glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_INT, 0);

	// 3. Отвязываем обратно для безопасности
	m_VAO.UnBind();
}

Mesh Mesh::CreateCube()
{
	Vertex vertices[] = {
		// Позиция                     // Текстура    // Нормаль
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  0.0f, -1.0f) },
		{ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(1.0f,  0.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
		{ glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  -1.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(0.0f,  1.0f, 0.0f) },
	};

	// Индексы для сборки 6 граней (каждая грань состоит из 2-х треугольников)
	unsigned int indices[] = {
		 0,  1,  2,   2,  3,  0,
		 4,  5,  6,   6,  7,  4,
		 8,  9, 10,  10, 11,  8,
		12, 13, 14,  14, 15, 12,
		16, 17, 18,  18, 19, 16,
		20, 21, 22,  22, 23, 20
	};

	// считаём сколько вершин у нас
	unsigned int vertexCount = sizeof(vertices) / sizeof(Vertex);

	// вовзращаем созданный меш(вершины, кол-во вершин, соединения, вес одного соединения)
	return Mesh(vertices, vertexCount, indices, sizeof(indices));
}