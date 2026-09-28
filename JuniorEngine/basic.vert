#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // Принимает UV-координаты из разметки VAO 
layout (location = 2) in vec3 aNormal; // Принимает вектор нормали из VAO

out vec2 TexCoord; // Отправляем эти координаты дальше во фрагментный шейдер
out vec3 Normal; // Отправляем нормаль во фрагментный шейдер
out vec3 FragPos; // Отправляем мировую позицию точки во фрагментный шейдер

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main()
{
   // Магия 3D-конвейера: умножаем позицию вершины на матрицы MVP.
   // ВАЖНО: умножение в GLSL идет строго СПРАВА НАЛЕВО!
    gl_Position = u_Projection * u_View * u_Model * vec4(aPos,1.0);

    // Пееводим позицию вершины в мировые коорлинаты(без проекции и камеры)
    FragPos = vec3(u_Model * vec4(aPos, 1.0));

    // Передаём нормаль во фрагментный шейдер.
    Normal = mat3(transpose(inverse(u_Model))) * aNormal;

    // Передаём UV-координаты пикселя без изменений
    TexCoord = aTexCoord;
}