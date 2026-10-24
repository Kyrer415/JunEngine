#include "Core/Scene.h"

Scene::Scene()
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
		SceneObject obj;
		obj.transform.Position = positions[i];

		obj.material = { 0.1f, 1.0f, 1.0f, 1.0f };

		m_Cubes.push_back(obj);
	}
}


void Scene::Update(float time)
{

}

