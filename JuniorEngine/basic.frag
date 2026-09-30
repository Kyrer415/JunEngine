#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Принимаем UV-координаты от вершинного щейдера
in vec3 Normal; // Принимаем нормаль из вершинного шейдера
in vec3 FragPos; // Принимаем мировую позицию точки

// Специальный тип данных для текстуры (Ссэмплер)
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec4 ourColor; // пулс цвет из c++

uniform vec3 u_LightColor; // цвет света от лампы :)
uniform vec3 u_LightPos; // Позици лампочки, которую мы передали через SetFloat3

void main()
{
	// Читаем цвет пикселя из смешаных текстур ue и opengl
	vec4 TexColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * ourColor;

	// 1. РАСЧЁТ AMBIENT (фоновое освещение)
	// Просто даём 10% базовой яркости (0.1), чтобы тени не были провально чёрными
	vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);

	// 2. РАСЧЁТ DIFFUSE (Диффузный свет)
	// Нормализуем входную нормаль для точности
	vec3 norm = normalize(Normal);

	// Вычисляем вектор направления от точки куба к лампочке
	vec3 lightDir = normalize(u_LightPos - FragPos);

	// Скалярное произведение (dot product) находит косинус угла для падения света.
	// max(..., 0.0) гарантирует, что если угол больше 90 градусов(свет светит сзади), значение не уйдёт в минус
	float diff = max(dot(norm, lightDir), 0.0);

	// Умножаем силу затухания на цвет лампы!
	vec3 diffuse = diff * u_LightColor;

	// 3. ФИНАЛЬНАЯ СБОРКА: складываем фоновый и диффузный свет
	vec3 resultLight = ambient + diffuse;

	// 4. Умножаем цвет текстуры на полученное освещение!
	FragColor = vec4(resultLight, 1.0) * TexColor;
}
