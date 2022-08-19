#include "Renderer.h"
#include "Camera.h"
#include"ImageLoader.h"
#include"MyModel.h"
#include"LightModel.h"

Renderer::Renderer()
{
	// Compute viewport and projection matrix
	GLint viewportSize[4];
	glGetIntegerv(GL_VIEWPORT, viewportSize);

	const float aspect_ratio = viewportSize[2] / (float)viewportSize[3];

	m_projection_xform = glm::perspective(glm::radians(90.0f), aspect_ratio, 1.0f, 400000.0f);

}

// Use IMGUI for a simple on screen GUI
void Renderer::DefineGUI(const Helpers::Camera& camera)
{
	// Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui::Begin("3GP");						// Create a window called "3GP" and append into it.

	ImGui::Text("Visibility.");					// Display some text (you can use a format strings too)	

	ImGui::Checkbox("Wireframe", &m_wireframe);	// A checkbox linked to a member variable

	ImGui::Checkbox("Ambient Lighting", &m_ambientLight);	// A checkbox linked to a member variable
	ImGui::Checkbox("Directional Lighting", &m_directionalLight);	// A checkbox linked to a member variable
	ImGui::Checkbox("Point Lighting", &m_pointLight);	// A checkbox linked to a member variable
	ImGui::Checkbox("Spot Lighting", &m_spotLight);	// A checkbox linked to a member variable


	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
}





// Render the scene. Passed the delta time since last called.
void Renderer::Render(const Helpers::Camera& camera, float deltaTime, const std::vector<std::shared_ptr<MyModel>>& models)
{			
	// Configure pipeline settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Wireframe mode controlled by ImGui
	if (m_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//// Clear buffers from previous frame
	glClearColor(0.0f, 0.0f, 0.0f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 view_xform = glm::lookAt(camera.GetPosition(), camera.GetPosition() + camera.GetLookVector(), camera.GetUpVector());
	glm::mat4 combined_xform = m_projection_xform * view_xform;
	
	for (const std::shared_ptr<MyModel>& model : models)
	{
		if (model->GetModelName() == "SkyBox")
			model->Render(m_projection_xform * glm::mat4(glm::mat3(view_xform)));
		else
		{
			if (model->GetModelName() == "AmbientLight")
			{
				if (m_ambientLight)
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(1);
				else
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(0);
			}
			else if (model->GetModelName() == "DirectionalLight")
			{
				if (m_directionalLight)
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(1);
				else
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(0);
			}
			else if (model->GetModelName() == "PointLight")
			{
				if (m_pointLight)
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(1);
				else
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(0);
			}
			else if (model->GetModelName() == "SpotLight")
			{
				if (m_spotLight)
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(1);
				else
					std::dynamic_pointer_cast<LightModel>(model)->ToggleLight(0);
			}
			model->Render(combined_xform);
		}
	}
}

