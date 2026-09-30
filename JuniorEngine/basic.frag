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
uniform vec3 u_LightPos; // Позиция лампочки, которую мы передали через SetFloat3
uniform vec3 u_ViewPos; // наша позиция камеры передаём через SetFloat3!

void main()
{
	// Читаем цвет пикселя из смешаных текстур ue и opengl
	vec4 TexColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2) * ourColor;

	// 1. РАСЧЁТ AMBIENT (фоновое освещение)
	// Просто даём 10% базовой яркости (0.1), чтобы тени не были провально чёрными
	vec3 ambient = 0.1 * vec3(1.0, 1.0, 1.0);

	// 2. РАСЧЁТ DIFFUSE (Диффузный свет - тени)
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_LightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;

	// 3. SPECILAR (Зеркальный блик!)
	float specularStrength = 0.5; // Сила блика (интенсивность)

	// Вычисляем вектор направления от точки куба к камере
	vec3 viewDir = normalize(u_ViewPos - FragPos);

	// Вычисляем вектор отражения луча света относительно нормали.
	// ВАЖНО!: тут reflect ожидает вектор ОТ источника света, поэтому ставим минус перед lightDir!
	vec3 reflectDir = reflect(-lightDir, norm);

	// Считаем косинус угла между отраженным лучом и вектором взгляда.
	// Возводим в 32-ю степень! Число 32 - это матовость\глянцевость (shininess)
	// Чем выше степень (32, 64, 128), тем меньше и острее будет точка блика.
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * u_LightColor;

	// СБОРКА ВСЕХ КОМПОНЕНТОВ ФОНГА! 
	vec3 resultLight = ambient + diffuse + specular;

	// Умножаем цвет текстуры на полученное освещение!
	FragColor = vec4(resultLight, 1.0) * TexColor;
}
