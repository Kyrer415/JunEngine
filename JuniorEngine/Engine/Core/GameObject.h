#pragma once

#include "Transform.h"
#include "Material.h"

class Model;

class GameObject
{
public:
	Transform transform;
	Material material;

	Transform& GetTransform() { return transform; }

	const Model* model = nullptr;
private:
};