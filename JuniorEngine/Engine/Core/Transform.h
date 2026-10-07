#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Transform
{
	glm::vec3 Position { 0.0f };
	glm::vec3 Rotation { 0.0f }; // Углы эйлера поворот X, Y, Z
	glm::vec3 Scale    { 1.0f };

	// Метод, который сам перемножает TRS (Translatem Rotate, Scale)
	// и возвращает готовую матрицу Model для вершинного шейдера!
	glm::mat4 GetModelMatrix() const
	{
		glm::mat4 model{ 1.0f };

			// 1. Translate (сдвиг)
			model = glm::translate(model, Position);

			// Rotate (поворот)
			model = glm::rotate(model, glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

			// 3. Scale ( Масштаб)
			model = glm::scale(model, Scale);

			return model;
	}
};