#pragma once

#include "VertexArray.h"	
#include <vector>

class Mesh
{
public:
	// Конструктор
	Mesh();

	// Конструктор, который сраззу принимает массив вершин и его размер в байтах
	Mesh(const float* vertices, unsigned int size);

	// Метод для загрузки или обновления данных
	void SetData(const float* verices, unsigned int size);

	// Метод дя отрисовки меша
	void Draw() const;

private:
	VertexArray m_VAO;
	unsigned int m_VertexCount; // Переменная, которая запомнит, сколько у нас вершин
};