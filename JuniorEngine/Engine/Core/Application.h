#pragma once

#include "Window\Window.h"
#include "Renderer\Shader.h" 
#include "Renderer\ShaderManager.h"
#include "Renderer\Renderer.h"
#include "Renderer\Model.h"
#include "Renderer\Texture.h"
#include "Renderer\Camera.h" 
#include "Core\CoreTime.h"
#include "Core\Input.h"
#include "Core\ImGuiManager.h"
#include "Core\Transform.h"
#include "Core\Scene.h"

#include <future>
#include <memory>

class Application
{
public:
	Application();
	~Application();
	
	void Run();

private:

	Window m_Window;
	Renderer m_Renderer;
	Camera m_Camera;
	ShaderManager m_ShaderManager;
	std::unique_ptr<Model> m_TableModel;
	std::future<std::vector<MeshData>> m_TableLoadingFuture;
	ImGuiManager m_ImGui;
	SceneSettings m_Settings;
	Model m_CubeModel;
	GameObject m_tableObject;
	Scene m_Scene;
	Texture m_WallTexture;
	Texture m_FaceTexture;
	Material m_TableMaterial;
};