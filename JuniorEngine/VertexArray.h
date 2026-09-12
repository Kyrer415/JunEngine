#pragma once

class VertexArray
{
public:
	// конструктор солздаст VAO и VBO на видеокарте
	VertexArray();

	// Деструктор очистит память вдиеокарты при удалении объекта
	~VertexArray();

	// Загрузить вершины в видеопамятьи настроть атрибуты 
	void SetData(const float* vertices, unsigned int size);

	// Активировать этот VertexArray для отрисовкеи
	void Bind() const;

	// Деактивировать (отвязать)
	void UnBind() const;

private:
	unsigned int m_VAO; // ID контейнера настроек
	unsigned int m_VBO; // ID буфера с сырыми данными вершин

};