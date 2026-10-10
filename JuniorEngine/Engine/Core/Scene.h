#pragma once

#include "Transform.h"
#include "Material.h"
#include <vector>

struct SceneObject
{
	Transform transform;
	Material material;
};

class Scene
{
public:
	Scene();
	void Update(float time);
	const std::vector<SceneObject>& GetCubes() const { return m_Cubes; }

private:
	std::vector<SceneObject> m_Cubes;
	SceneObject m_Table;
};