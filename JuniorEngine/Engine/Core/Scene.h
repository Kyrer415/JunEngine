#pragma once

#include "Transform.h"
#include "Material.h"
#include <vector>
#include "GameObject.h"
#include <memory>

class Scene
{
public:
	Scene(const Model& cubeModel);
	void Update(float time);
	const std::vector<GameObject>& GetObjects() const { return m_Objects; }
	const std::vector<std::shared_ptr<GameObject>>& GetModObjects() const { return m_ModObjects; }

	void AddObject(std::shared_ptr<GameObject> object);

	void SetCubeScaleY(float settings);

private:
	std::vector<GameObject> m_Objects;
	std::vector<std::shared_ptr<GameObject>> m_ModObjects;
};