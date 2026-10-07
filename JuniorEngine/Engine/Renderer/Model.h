#pragma once

#include <string>
#include <vector>
#include "Renderer/Mesh.h" // тут лежат структура вершин vertex и класс mesh
#include "Core/Transform.h"
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

	Transform& GetTransform() { return m_Transform; }

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
	Transform m_Transform;
};

