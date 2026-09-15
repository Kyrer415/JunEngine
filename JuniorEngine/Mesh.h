#pragma once

#include "VertexArray.h"	
#include <vector>

class Mesh
{
public:
	// Конструктор
	Mesh();

	// Конструктор, который сраззу принимает массив вершин и его размер в байтах
	Mesh(const float* vertices, unsigned int vSize, const unsigned int* indices, unsigned int iSize);

	// Метод для загрузки или обновления данных
	void SetData(const float* vertices, unsigned int vSize, const unsigned int* indices, unsigned int iSize);

	// Метод дя отрисовки меша
	void Draw() const;

private:
	VertexArray m_VAO;
	unsigned int m_IndexCount; // Переменная, которая запомнит, сколько у нас соединений
};