#include "Renderer\Shader.h"
#include "Core/Material.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& filepath)
	: m_ID(0)
{

	ShaderSourceProgram source = ParseShader(filepath);

	// читаем исходный код шейдеров из файлов на диске
	std::string vertexCode = source.VertexSource;
	std::string fragmentCode = source.FragmentSource;

	// Если файлы успешно прочитаны, компилируем их
	if (!vertexCode.empty() && !fragmentCode.empty())
	{
		Compile(vertexCode, fragmentCode);
	}
}

Shader::~Shader()
{
	// Удаляем програму с видеокарты, когда объект класса уничтожается
	if (m_ID != 0)
	{
		glDeleteProgram(m_ID);
	}
}

Shader::Shader(Shader&& o) noexcept
	:m_ID(o.m_ID),
	 m_UniformCache(std::move(o.m_UniformCache))
{
	o.m_ID = 0;
}

Shader& Shader::operator=(Shader&& o) noexcept
{
	if (this != &o)
	{
		if (m_ID) glDeleteProgram(m_ID);
		m_ID = o.m_ID;
		o.m_ID = 0;
		m_UniformCache = std::move(o.m_UniformCache);
	}
	return *this;
}

void Shader::Use() const
{
	// Активируем шейдерную программу для отрисвки
	glUseProgram(m_ID);
}

void Shader::Compile(const std::string& vertexCode, const std::string& fragmentCode)
{
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// 1. Компиляция вершинного шейдера
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Компиляция Вершинного Шейдера внутри класса провалена: \n" << infoLog << std::endl;
	}

	// 2. Компиляция фрагментного шейдреа
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Компиляция фрагментного шейдера внутри класса провалена: \n" << infoLog << std::endl;
	}

	// 3. Создание шейдерной программы и линковка
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ID, 512, nullptr, infoLog);
		std::cerr << "ОШИБКА: Линковка Шейдерной Программы ванутри класса провалена: \n" << infoLog << std::endl;
	}

	// Удаляем шейдера, они нам больше не нужны, так как они уже связаны в программе m_ID
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::SetFloat4(const std::string& name, float v0, float v1, float v2, float v3) const
{
	// 1. Находим "адрес" переменной по нашему методу
	int vertexColorLocation = GetUniformLocation(name);

	// 2. Если переменная найдена, загружаем в неё 4 наших значения
	if (vertexColorLocation != -1)
	{
		glUniform4f(vertexColorLocation, v0, v1, v2, v3);
	}
}

void Shader::SetFloat3(const std::string& name, float v0, float v1, float v2) const
{
	// 1. Находим "адрес" переменной по нашему методу
	int Location3 = GetUniformLocation(name);

	// 2. Если переменная найдена, загружаем в неё 3 наших значения
	if (Location3 != -1)
	{
		glUniform3f(Location3, v0, v1, v2);
	}
}

void Shader::SetFloat(const std::string& name, float value) const
{
	// 1. Находим "адрес" с помощью нашего метода
	int Location = GetUniformLocation(name);

	if (Location != -1)
	{
		glUniform1f(Location, value);
	}
}

int Shader::GetUniformLocation(const std::string& name) const
{
	// 1. Проверяем, есть ли уже это имя в кэше
	auto it = m_UniformCache.find(name);
	if (it != m_UniformCache.end())
	{
		return it->second; // Нашли! Мнгновенно возвращаем сохранённый ID ячейки
	}


	// 2. Если в кеше нет, справшивает у видеокарты
	int Location = glGetUniformLocation(m_ID, name.c_str());

	if (Location == -1)
	{
		std::cerr << "Uniform not found: " << name << "\n";
	}

	// 3. Запоминаем в кэш
	m_UniformCache[name] = Location;
	return Location;
}

void Shader::SetMatrix4(const std::string& name, const glm::mat4& matrix) const
{
	int Location = GetUniformLocation(name); // наш оптимизированный кеш метод получения имени порта
	if (Location != -1) // если он равен -1 значит порта несуществует 
	{
		// Передаём матрицу 4x4 на видеокарту
		glUniformMatrix4fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}

void Shader::SetMatrix3(const std::string& name, const glm::mat3& matrix) const
{
	int Location = GetUniformLocation(name); // наш оптимизированный кеш метод получения имени порта
	if (Location != -1) // если он равен -1 значит порта несуществует 
	{
		// Передаём матрицу 3x3 на видеокарту
		glUniformMatrix3fv(Location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
}


void Shader::SetInt(const std::string& name, int value) const
{
	int Location = GetUniformLocation(name);
	if (Location != -1)
	{
		glUniform1i(Location, value);
	}
}

void Shader::SetMaterial(const std::string& name, const Material& m) const
{
	SetFloat(name + ".ambient", m.ambient);
	SetFloat(name + ".diffuse", m.diffuse);
	SetFloat(name + ".specular", m.specular);
	SetFloat(name + ".shininess", m.shininess);
}

ShaderSourceProgram Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);
	
	if (!stream.is_open())
		std::cerr << "ОШИБКА: не удалось открыть шейдер по пути: " << filepath << "\n";

	// Перечисления для отслеживание, в какой буфер сейчас пишем
	enum class ShaderType { NONE = -1, VERTEX = 0, FRAGMENT = 1 };

	std::string line;
	std::stringstream ss[2]; // ss[0] для вершинного, ss[1] для фрагментного
	ShaderType type = ShaderType::NONE;

	while (std::getline(stream, line))
	{
		// Если нашли тег начала вершинного шейдера
		if (line.find("#shader vertex") != std::string::npos)
		{
			type = ShaderType::VERTEX;
		}
		// Если нашли тег начала фрагментного шейдера
		else if (line.find("#shader fragment") != std::string::npos)
		{
			type = ShaderType::FRAGMENT;
		}
		else
		{
			// Если мы уже внутри какого-то режима - пишем строку в соответствуюбщий стрим
			if (type != ShaderType::NONE)
			{
				ss[(int)type] << line << '\n';
			}
		}
	}
	return { ss[0].str(), ss[1].str() };
}