#pragma once

#include <string>
#include <memory>

struct StbiDeleter { void operator()(unsigned char* p) const; }; 

struct ImageData
{
	std::unique_ptr<unsigned char, StbiDeleter> Pixels;
	int Width{ 0 };
	int Height{ 0 };
	int Channels{ 0 };
};

class Texture
{
public:
	// Конструктор принимает структуру данных(пиксели, размеры, каналы)
	Texture(const ImageData& data);
	~Texture();

	// ЗАпрещаем копирование
	Texture(const Texture&) = delete; // Копировать нельзя!
	Texture& operator=(const Texture&) = delete; // присваивать копию нельзя!

	// Разрешаем перемещение
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	// Включить текстуру перед отрисовкоай меша. slot - это номер текстурного юнита (от 0 до 15)
	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	static ImageData LoadImageFromDisk(const std::string& filePath);

	static void SetFlipVerticallyOnLoad(bool flip);

private:
	unsigned int m_ID; // Уникальный ID тексуры на видеокарте
	int m_Width, m_Height; // Размеры картинки в пикселях
	int m_Channels; //  кол-во каналов цвета например RGB или RGBA
};