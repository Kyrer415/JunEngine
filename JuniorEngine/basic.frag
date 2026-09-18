#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Принимаем UV-координаты от вершинного щейдера

// Специальный тип данных для текстуры (Ссэмплер)
uniform sampler2D ourTexture;
uniform vec4 ourColor;

void main()
{
	// Функция texture() берёт картинку и вытаскивает из неё цвет пикселя по координатам TexCoord.
	// А умножение на ourColor заставит красиво пульсировать или менять оттенок!
	FragColor = texture(ourTexture, TexCoord) * ourColor;
}
