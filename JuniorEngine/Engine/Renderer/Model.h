#pragma once

#include <string>
#include <vector>
#include "Mesh.h" // тут лежат структура вершин vertex и класс mesh
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Shader;

class Model
{
public:
	// конструктор: принимает путь к 3д модели
	Model(const std::string& path);

	// Метож отрисовки всей модели (она может состоять из нескольких мешей)
	void Draw(const Shader& shader) ;

private:
	// Функция загрузки модели через Assimp
	void LoadModel(const std::string& path);

	// Рекурсивный обход всех узлов (нод) 3д-файла
	void ProcessNode(aiNode* node, const aiScene* scene);

	// превращение меша Assimp в наш собственный объект класса Mesh
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

private:
	// Модель может состоять ищ мнжества отдельных кусков
	std::vector<Mesh> m_Meshes;
	std::string m_Directory; // Папка, где лежит модель ( нужна, чтобы искать текстуры рядом)
};

