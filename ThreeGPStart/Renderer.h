#pragma once

#include "ExternalLibraryHeaders.h"

#include "Helper.h"
#include "Mesh.h"
#include "Camera.h"

class MyModel;

class Renderer
{
private:

	bool m_wireframe{ false },
		m_ambientLight,
		m_directionalLight,
		m_pointLight,
		m_spotLight;;

	glm::mat4 m_projection_xform;

public:
	Renderer();
	~Renderer() = default;

	// Draw GUI
	void DefineGUI(const Helpers::Camera& camera);

	void Render(const Helpers::Camera& camera, float deltaTime, const std::vector<std::shared_ptr<MyModel>>& models);
};

