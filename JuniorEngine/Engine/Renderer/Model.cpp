#include "Model.h"
#include <iostream>

Model::Model(const std::string& path)
{
	LoadModel(path);
}

void Model::Draw(const Shader& shader) 
{
	// Проходимся по все мешам нашей модели и заставляем каждый отрисоваться
	for ( auto& mesh : m_Meshes)
	{
		mesh.Draw(); // у класса Mesh должен быть метод Draw
	}
}

void Model::LoadModel(const std::string& path)
{
	Assimp::Importer importer;

	// Читаем файл и применяем флаги постобработки:
	// aiProcess_Triangulate - принудительно превращает любые полигоны (квадраты и т.д) в треугольники
	// aiProcess_FlipUvs - зеркалит текстурные координаты по вертикали так как в OpenGL Y инвертирован
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	// Проверяем на ошибки загрузки
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ОШИБКА::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// Извлекаем путь к папке, в которой лежит файл модели
	m_Directory = path.substr(0, path.find_last_of('/'));

	// Запускаем рекурсивную обработку, начиная с самого главного (корневого) узла сцены
	ProcessNode(scene->mRootNode, scene);

	std::cout << "УСПЕХ::Модель успешно загружена из:" << path << std::endl;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// 1. Обрабатываем все меши, которые висят на текущем узлле (ноде)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}

	// 2. РЕКУРСИЯ: Спускаемся глубже во всех детей текущего узла
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	
	// 1. Перекачиваем ВЕРШИНЫ
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Поз (XYZ)
		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		// Нормали (nX,nY,nZ)
		if (mesh->HasNormals())
		{
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
		}

		// Текстурные координаты (UV)
		if (mesh->mTextureCoords[0]) // есть у меша текстура?0
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);		
	}

	// 2. Перекачиваем ИНДЕКСЫ (сборка треугольников)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	
	// Возвращаем готовый скомпилированный меш 
	return Mesh(vertices, indices);
}
