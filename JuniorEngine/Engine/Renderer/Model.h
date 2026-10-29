#pragma once

#include <string>
#include <vector>
#include "Renderer/Mesh.h" // тут лежат структура вершин vertex и класс mesh
#include "Renderer/Texture.h"
#include "Core/Transform.h"

class Shader;
struct aiScene;
struct aiMesh;

struct MeshData
{
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<ImageData> TexturesData; // Сырые Cpu_данные картинок для меша стола
};

class Model
{
public:
	// конструктор: принимает нашу структуру меша
	Model(std::vector<MeshData>&& modelData);

	Model(Mesh&& sigleMesh) { m_Meshes.push_back(std::move(sigleMesh)); }

	// Метод отрисовки всей модели (она может состоять из нескольких мешей)
	const void Draw() const;

	static std::vector<MeshData> LoadModelFromDisk(const std::string& path);

private:
	// превращение меша Assimp в наш собственный объект класса Mesh
	static MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory);

private:
	// Модель может состоять ищ мнжества отдельных кусков
	std::vector<Mesh> m_Meshes;
};

