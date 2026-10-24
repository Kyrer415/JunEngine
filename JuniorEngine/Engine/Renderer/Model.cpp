#include "Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>
#include <filesystem>
#include <iostream>

Model::Model(std::vector<MeshData>&& modelData)
{
	// Резервируем память в векторе мешей, чтобы избежать лишних перевыделений
	m_Meshes.reserve(modelData.size());

	// Пробегаемся по всем кускм CPU-данных, которые нам прислали
	for (auto& meshData : modelData)
	{
		// Собираем список реальных GPU-текстур из сырых картинок ImageData
		std::vector<Texture> gpuTextures;
		gpuTextures.reserve(meshData.TexturesData.size());

		for (auto& imageData : meshData.TexturesData)
		{
			// Создаём объект текстуры во VRAM, передавая туда пиксели.
			gpuTextures.emplace_back(imageData);
			imageData.Pixels.reset();
		}


		// Собираем готовый Mesh в видеопамяти и пушим его в вектор модели
		// передаём вергины, индексы и сгенерированные текстуры!
		m_Meshes.emplace_back(meshData.Vertices, meshData.Indices, std::move(gpuTextures));
	}
}

void Model::Draw()
{
	// Проходимся по все мешам нашей модели и заставляем каждый отрисоваться
	for (auto& mesh : m_Meshes)
	{
		mesh.Draw(); // у класса Mesh должен быть метод Draw
	}
}

std::vector<MeshData> Model::LoadModelFromDisk(const std::string& path)
{
	Assimp::Importer importer;

	// Читаем файл и применяем флаги постобработки:
	// aiProcess_Triangulate - принудительно превращает любые полигоны (квадраты и т.д) в треугольники
	// aiProcess_FlipUvs - зеркалит текстурные координаты по вертикали так как в OpenGL Y инвертирован
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);

	// Проверяем на ошибки загрузки
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "ОШИБКА::ASSIMP:: " << importer.GetErrorString() << std::endl;
		throw std::runtime_error(importer.GetErrorString());
	}

	std::string directory = std::filesystem::path(path).parent_path().string();
	std::vector<MeshData> modelData;

	// вспомогательная лямбда-функция для рекурсивного оюхода нод (чтобы не плодить мтеоды в хедере)
	auto processNode = [&](aiNode* node, const aiScene* scene, auto& self) -> void
		{
			for (unsigned int i = 0; i < node->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

				// Вызываем парсинг меша
				modelData.push_back(ProcessMesh(mesh, scene, directory));
			}
			for (unsigned int i = 0; i < node->mNumChildren; i++)
			{
				self(node->mChildren[i], scene, self);
			}
		};

	// Запускаем рекурсивную обработку, начиная с самого главного (корневого) узла сцены
	processNode(scene->mRootNode, scene, processNode);

	std::cout << "УСПЕХ::Модель успешно загружена из:" << path << std::endl;
	return modelData;
}

MeshData Model::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::string& directory)
{
	MeshData meshData;

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
		if (mesh->mTextureCoords[0]) // есть у меша текстура?
		{
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		meshData.Vertices.push_back(vertex);
	}

	// 2. Перекачиваем ИНДЕКСЫ (сборка треугольников)
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			meshData.Indices.push_back(face.mIndices[j]);
		}
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	// Проверяем, есть ли у материала диффузные текстуры (карта цвета)
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString str;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &str);

		// Склеиваем путь к папке модели и имя файла текстуры 
		std::string texturePath = directory + "/" + str.C_Str();

		// пушим путь в локальный вектор меша!
		ImageData imgData = Texture::LoadImageFromDisk(texturePath);
		meshData.TexturesData.push_back(std::move(imgData));

	}

	// Возвращаем готовую кпу-структуру меша
	return meshData;
}

