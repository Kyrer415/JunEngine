#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // Принимает UV-координаты из разметки VAO

// Принимаем угол поворота в радианах из C++
uniform float u_Angle; 

out vec2 TexCoord; // Отправляем эти координаты дальше во фрагментный шейдер

void main()
{
    // Матрица вращения "на коленке" вокруг оси Z:
    // Изменяем X и Y координаты с помощью синуса и колсинуса угла
    float cosA = cos(u_Angle);
    float sinA = sin(u_Angle);

    float newX = aPos.x * cosA - aPos.y * sinA;
    float newY = aPos.x * sinA + aPos.y * cosA;

    gl_Position = vec4(newX, newY, aPos.z, 1.0);

    // Передаём UV-координаты пикселя без изменений
    TexCoord = aTexCoord;
}
