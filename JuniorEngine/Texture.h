#pragma once

#include <string>

class Texture
{
public:
	// Конструктор принимает путь к файлу картинки 
	Texture(const std::string& filePath);
	~Texture();

	// Включить текстуру перед отрисовкоай меша. slot - это номер текстурного юнита (от 0 до 15)
	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

private:
	unsigned int m_ID; // Уникальный ID тексуры на видеокарте
	std::string m_FilePath; // путь к файлу
	int m_Width, m_Height; // Размеры картинки в пикселях
	int m_BPP; // Bits Per Pixel (кол-во каналов цвета например RGB или RGBA
};