#define STB_IMAGE_IMPLEMENTATION
#include "ThirdParty\stb_image.h"

#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const std::string& filePath)
	: m_ID(0), m_FilePath(filePath), m_Width(0), m_Height(0), m_BPP(0)
{
	// OpenGL считывает пиксели снизу вверх, а картинки на диске хранятся сверху вниз.
	// Эта строчка переворачивает картинку при загрузке, чтобы она была вверх ногами!
	stbi_set_flip_vertically_on_load(1);
	
	// Загружаем пиксели картинки в оперативную памчять(CPU)
	// 4. означает что мы принудительно просим выдать нам 4 канала (RGBA)
	unsigned char* LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	if (!LocalBuffer)
	{
		std::cerr << "Ошибка: Не удалось загрузить текстуру по пути:" << filePath << std::endl;
		return;
	}

	// Генерируем ID для текстуры на видеократе
	glGenTextures(1, &m_ID);
	glBindTexture(GL_TEXTURE_2D, m_ID);

	// Настройка параметров фильтрации (как сглаживать картинку при растяжении)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Настройка повторения ( что делать, если координаты вышли за пределы от 0 до 1)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ВАЖНО: Копируем пиксели из оперативки (LocalBuffer) в видеопамять (GPU)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, LocalBuffer);

	// отвзяываем текстуру
	glBindTexture(GL_TEXTURE_2D, 0);
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