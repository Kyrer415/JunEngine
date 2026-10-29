#include "Core/Scene.h"
#include <glm/glm.hpp>


Scene::Scene(const Model& CubeModel)
{
	const glm::vec3 positions[] = {
	{ 0.0f,  0.0f,   0.0f }, { 2.0f,  5.0f, -15.0f },
	{-1.5f, -2.2f,  -2.5f }, {-3.8f, -2.0f, -12.3f },
	{ 2.4f, -0.4f,  -3.5f }, {-1.7f,  3.0f,  -7.5f },
	{ 1.3f, -2.0f,  -2.5f }, { 1.5f,  2.0f,  -2.5f },
	{ 1.5f,  0.2f,  -1.5f }, {-1.3f,  1.0f,  -1.5f }
	};

	for (size_t i = 0; i < std::size(positions); i++)
	{
		GameObject obj;
		obj.transform.Position = positions[i];

		obj.material = { 0.1f, 1.0f, 1.0f, 1.0f };

		obj.model = &CubeModel;

		obj.transform.Rotation = glm::vec3(1.0f, 0.3f, 0.5f) * (20.0f * i);

		m_Objects.push_back(obj);
	}
}

void Scene::AddObject(const GameObject& object)
{
	m_Objects.push_back(object);
}

void Scene::Update(float dt)
{
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i].transform.Rotation += glm::vec3(1.0f, 0.3f, 0.5f) * (50.0f * dt);
	}
}

void Scene::SetCubeScaleY(float scale)
{
	for (auto &obj : m_Objects)
	{
		obj.transform.Scale.y = scale;
	}
}