#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Принимаем UV-координаты от вершинного щейдера

// Специальный тип данных для текстуры (Ссэмплер)
uniform sampler2D texture1;
uniform sampler2D texture2;

uniform vec4 ourColor;

void main()
{
	// 1. Читаем цвет пикселя из первой текстуры
	vec4 color1 = texture(texture1, TexCoord);

	// 2. Читаем цвет пикселя из второй текстуры
	vec4 color2 = texture(texture2, TexCoord);

	// 3. Смешиваем с помощью mix()! 
	// mix(A, B, X) линейно интерполирует между A и B.
	// 0.2 означает, что итоговый цвет будет состоять на 80% из первой картинки и на 20% из второй.
	vec4 blendedTexture = mix(color1, color2, 0.2);

	// 4. Умножаем на наш пульсирующий цвет
	FragColor = blendedTexture* ourColor;
}
