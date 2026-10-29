#pragma once

#include "Transform.h"
#include "Material.h"
#include <vector>
#include "GameObject.h"

class Scene
{
public:
	Scene(const Model& cubeModel);
	void Update(float time);
	const std::vector<GameObject>& GetObjects() const { return m_Objects; }

	void AddObject(const GameObject& object);

	void SetCubeScaleY(float settings);

private:
	std::vector<GameObject> m_Objects;
};