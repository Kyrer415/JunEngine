#pragma once

#include <glm/glm.hpp>

struct GLFWwindow;

struct SceneSettings
{
	float Shininess{ 32.0f };
	float lightRadius{ 2.5f };
	float lightOrbitSpeed{ 1.0f };
	float lightHeight{ 2.0f };
	glm::vec3 lightColorIntensity{ 1.0f };
	float CubeScale{ 1.0f };
	bool isDiscoMode{ false };
	bool isUIFocused = false; // По умолчанию мы в режиме полёта(курсор зафиксирован и спрятан)
	bool altKeyReleased = true; // Предохранитель от зажимания

};

class ImGuiManager
{
public:

	ImGuiManager(GLFWwindow* nativeWindow);

	~ImGuiManager();

	ImGuiManager(const ImGuiManager&) = delete;
	ImGuiManager& operator=(const ImGuiManager&) = delete;
	ImGuiManager(ImGuiManager&& other) = delete;
	ImGuiManager& operator=(ImGuiManager&& other) = delete;

	void BeginFrame();

	void EndFrame();

	void RenderUI(SceneSettings& settings, struct Transform& TableTransform);

private:
	
};