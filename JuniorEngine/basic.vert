#version 330 core
layout (location = 0) in vec3 aPos;

// Принимаем угол поворота в радианах из C++
uniform float u_Angle; 

void main()
{
    // Матрица вращения "на коленке" вокруг оси Z:
    // Изменяем X и Y координаты с помощью синуса и колсинуса угла
    float cosA = cos(u_Angle);
    float sinA = sin(u_Angle);

    float newX = aPos.x * cosA - aPos.y * sinA;
    float newY = aPos.x * sinA + aPos.y * cosA;


    gl_Position = vec4(newX, newY, aPos.z, 1.0);
}
