#pragma once

#include "ExternalLibraryHeaders.h"
#include "Camera.h"

class Renderer;
struct GLFWwindow;
class MyModel;

// Simulation class to handle input, updating of the simulation and calling the renderer
class Simulation
{
private:
	// A simple camera
	std::shared_ptr<Helpers::Camera> m_camera;

	// The renderer
	std::shared_ptr<Renderer> m_renderer;

	void InitEntities();
	std::vector<std::shared_ptr<MyModel>> m_models{};

	// Remember last update time so we can calculate delta time
	float m_lastTime{ 0 };

	// Handle any user input. Return false if program should close.
	bool HandleInput(GLFWwindow* window);

	GLuint
		m_program,
		m_cubeProgram,
		m_skyBoxProgram;
	
	void CreateShaders();
	void CreateShader(GLuint& programID, const std::string& vertexShaderShaderFile, const std::string& fragmentShaderFile);


public:
	// Initialise this as well as the renderer, returns false on error
	bool Initialise();	

	// Update the simulation (and render) returns false if program should clse
	bool Update(GLFWwindow* window);

	~Simulation();
	
};

