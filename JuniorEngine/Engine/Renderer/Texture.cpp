#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty\stb_image.h"

#include "Renderer\Texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const ImageData& data)
	: m_ID(0), m_Width(data.Width), m_Height(data.Height), m_Channels(data.Channels)
{
	if (!data.Pixels) { std::cerr << "Ошибка: Текстура равна нулю!" << std::endl; return; }

	// Генерируем ID для текстуры на видеократе
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// Настройка параметров фильтрации (как сглаживать картинку при уменьшении) мипмапы как лоды только для текстур
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Настройка повторения ( что делать, если координаты вышли за пределы от 0 до 1)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ВАЖНО: Копируем пиксели из оперативки (LocalBuffer) в видеопамять (GPU)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.Pixels.get());

	glGenerateMipmap(GL_TEXTURE_2D);   // видеокарта сама построит все уровни

	// отвзяываем текстуру
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(Texture&& o) noexcept
	: m_ID(o.m_ID), 
	m_Width(o.m_Width), 
	m_Height(o.m_Height), 
	m_Channels(o.m_Channels)
{
	o.m_ID = 0;
}

Texture& Texture::operator=(Texture&& o) noexcept
{
	if (this != &o)
	{
		if (m_ID != 0) glDeleteTextures(1, &m_ID);
		m_ID = o.m_ID;
		m_Width = o.m_Width; m_Height = o.m_Height; m_Channels = o.m_Channels;
		o.m_ID = 0;
	}
	return *this;
}

Texture::~Texture()
{
	if (m_ID != 0)
	{
		glDeleteTextures(1, &m_ID);
	}
}

void Texture::Bind(unsigned int slot) const
{
	// Активируем нуужный текстурный слот (текстурный юнит)
	glActiveTexture(GL_TEXTURE0 + slot);
	// Привязываем нашу текстуру к этому слоту
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

ImageData Texture::LoadImageFromDisk(const std::string& filePath)
{
	ImageData data;
	// Флип включается один раз в main (Texture::SetFlipVerticallyOnLoad), это глобальная настройка stb
	// Загружаем пиксели картинки в оперативную памчять(CPU)
	// 4. означает что мы принудительно просим выдать нам 4 канала (RGBA)
	data.Pixels.reset(stbi_load(filePath.c_str(), &data.Width, &data.Height, &data.Channels, 4));

	if (!data.Pixels)
	{
		std::cerr << "Ошибка: Не удалось загрузить текстуру по пути: " << filePath << std::endl;
	}
	return data;
}

void StbiDeleter::operator()(unsigned char* p) const
{
	stbi_image_free(p);
}

void Texture::SetFlipVerticallyOnLoad(bool flip)
{
	stbi_set_flip_vertically_on_load(flip ? 1 : 0);
}