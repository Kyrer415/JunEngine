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
Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices, const std::vector<Texture>& textures)
{
	m_IndexCount = indices.size();
	m_Textures = textures; // копируем текстуры внутрь меша

	m_VAO.SetData(vertices.data(), vertices.size(), indices.data(), indices.size());
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