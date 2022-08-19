#include "Simulation.h"
#include "Camera.h"
#include "Renderer.h"

#include"MyModel.h"
#include"CubeModel.h"
#include"AquaPigModel.h"
#include"SkyBoxModel.h"
#include"TerrainModel.h"
#include"LightModel.h"

// Initialise this as well as the renderer, returns false on error
bool Simulation::Initialise()
{
	// Set up camera
	m_camera = std::make_shared<Helpers::Camera>();
	m_camera->Initialise(glm::vec3(0, 1800, 0), glm::vec3(0));

	// Set up renderer
	m_renderer = std::make_shared<Renderer>();

	InitEntities();

	return true;;
}

void Simulation::InitEntities()
{
	CreateShaders();

	//SKY BOX
	{
		std::shared_ptr<SkyBoxModel> skyBox{ std::make_shared<SkyBoxModel>(m_skyBoxProgram, "SkyBox") };
		skyBox->LoadModelData("Data\\Models\\Sky\\Hills\\", "skybox", ".x", "", "");
		skyBox->InstansiateModel();
		m_models.emplace_back(skyBox);
	}

	//ARMY TRUCK
	{
		std::shared_ptr<MyModel> truck{ std::make_shared<MyModel>(m_program, "Jeep", glm::vec3(200, 740, -200)) };
		truck->LoadModelData("Data\\Models\\Jeep\\", "jeep", ".obj", "jeep_army", ".jpg");
		truck->InstansiateModel();
		m_models.emplace_back(truck);
	}

	//RED TRUCK
	{
		std::shared_ptr<MyModel> truck2{ std::make_shared<MyModel>(m_program, "Jeep", glm::vec3(600, 740, -2200)) };
		truck2->LoadModelData("Data\\Models\\Jeep\\", "jeep", ".obj", "jeep_rood", ".jpg");
		truck2->InstansiateModel();
		m_models.emplace_back(truck2);
	}

	//CUBE
	{
		std::shared_ptr<CubeModel> cube{ std::make_shared<CubeModel>(m_cubeProgram, "Cube", glm::vec3(0, 1200,0), glm::vec3(0, 0, 0), glm::vec3(5, 5, 5)) };
		cube->InstansiateModel();
		m_models.emplace_back(cube);
	}

	//AQUA PIG
	{
		std::shared_ptr<AquaPigModel> aquaPig{ std::make_shared<AquaPigModel>(m_program, "AquaPig", glm::vec3(700, 800, -1200), glm::vec3(0, 0, 0), glm::vec3(80, 80, 80)) };
		aquaPig->LoadModelData("Data\\Models\\AquaPig\\", "hull", ".obj", "aqua_pig_2K", ".png");
		aquaPig->LoadModelData("Data\\Models\\AquaPig\\", "wing_left", ".obj", "aqua_pig_2K", ".png");
		aquaPig->LoadModelData("Data\\Models\\AquaPig\\", "wing_right", ".obj", "aqua_pig_2K", ".png");
		aquaPig->LoadModelData("Data\\Models\\AquaPig\\", "propeller", ".obj", "aqua_pig_2K", ".png");
		aquaPig->LoadModelData("Data\\Models\\AquaPig\\", "gun_base", ".obj", "aqua_pig_2K", ".png");
		aquaPig->LoadModelData("Data\\Models\\AquaPig\\", "gun", ".obj", "aqua_pig_2K", ".png");
		aquaPig->InstansiateModel();
		m_models.emplace_back(aquaPig);
	}

	//TERRAIN
	{
		std::shared_ptr<TerrainModel> terrainModel{ std::make_shared<TerrainModel>(m_program, "Terrain") };
		terrainModel->InstansiateModel();
		m_models.emplace_back(terrainModel);
	}

	//AMBIENT LIGHT
	{
		std::shared_ptr<LightModel> ambientLight{ std::make_shared<LightModel>(
			m_program,
			"AmbientLight",
			0,
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.15f,
			LightType::Ambient
			) };
		m_models.emplace_back(ambientLight);
	}

	//DIRECTIONAL LIGHT
	{
		std::shared_ptr<LightModel> directionalLight{ std::make_shared<LightModel>(
			m_program,
			"DirectionalLight",
			0,
			glm::vec3(-1.0f, -1.0f, 0.0f),
			glm::vec3(1.0f, 1.0f, 0.0f),
			0.5f,
			LightType::Directional
			) };
		m_models.emplace_back(directionalLight);
	}

	//SPOT LIGHTS
	{
		std::shared_ptr<LightModel> spotLight{ std::make_shared<LightModel>(
		m_program,
		"SpotLight",
		1,
		glm::vec3(0, 800, 0),
		glm::vec3(20, 20, 20),
		glm::vec3(0, -1, 0),
		glm::vec3(1, 1, 1),
		1.0f,
		3000.f,
		1.1f,
		LightType::SpotLight

		) };
		spotLight->LoadModelData("Data\\Models\\Sphere\\", "sphere", ".obj", "spotLightDiffuse", ".png");
		spotLight->InstansiateModel();
		m_models.emplace_back(spotLight);

		std::shared_ptr<LightModel> spotLight2{ std::make_shared<LightModel>(
		m_program,
		"SpotLight",
		2,
		glm::vec3(1000, 800, 0),
		glm::vec3(20, 20, 20),
		glm::vec3(0, -1, 0),
		glm::vec3(1, 1, 1),
		1.0f,
		3000.f,
		1.1f,
		LightType::SpotLight

		) };
		spotLight2->LoadModelData("Data\\Models\\Sphere\\", "sphere", ".obj", "spotLightDiffuse", ".png");
		spotLight2->InstansiateModel();
		m_models.emplace_back(spotLight2);


		std::shared_ptr<LightModel> spotLight3{ std::make_shared<LightModel>(
		m_program,
		"SpotLight",
		3,
		glm::vec3(2000, 800, 0),
		glm::vec3(20, 20, 20),
		glm::vec3(0, -1, 0),
		glm::vec3(1, 1, 1),
		1.0f,
		3000.f,
		1.1f,
		LightType::SpotLight

		) };
		spotLight3->LoadModelData("Data\\Models\\Sphere\\", "sphere", ".obj", "spotLightDiffuse", ".png");
		spotLight3->InstansiateModel();
		m_models.emplace_back(spotLight3);
	}

	//POINT LIGHTS
	{
		std::shared_ptr<LightModel> pointLight{ std::make_shared<LightModel>(
		m_program,
		"PointLight",
		4,
		glm::vec3(-2000, 800, 2000),
		glm::vec3(20, 20, 20),
		glm::vec3(1, 1, 1),
		1.0f,
		3000.f,
		LightType::PointLight

	) };
		pointLight->LoadModelData("Data\\Models\\Sphere\\", "sphere", ".obj", "pointLightDiffuse", ".png");
		pointLight->InstansiateModel();
		m_models.emplace_back(pointLight);

		std::shared_ptr<LightModel> pointLight2 {std::make_shared<LightModel>(
		m_program,
		"PointLight",
		5,
		glm::vec3(0, 800, 2000),
		glm::vec3(20, 20, 20),
		glm::vec3(1, 1, 1),
		1.0f,
		3000.f,
		LightType::PointLight

		) };
		pointLight2->LoadModelData("Data\\Models\\Sphere\\", "sphere", ".obj", "pointLightDiffuse", ".png");
		pointLight2->InstansiateModel();
		m_models.emplace_back(pointLight2);


		std::shared_ptr<LightModel> pointLight3{ std::make_shared<LightModel>(
		m_program,
		"PointLight",
		6,
		glm::vec3(2000, 800, 2000),
		glm::vec3(20, 20, 20),
		glm::vec3(1, 1, 1),
		1.0f,
		3000.f,
		LightType::PointLight

		) };
		pointLight3->LoadModelData("Data\\Models\\Sphere\\", "sphere", ".obj", "pointLightDiffuse", ".png");
		pointLight3->InstansiateModel();
		m_models.emplace_back(pointLight3);
	}
}

// Handle any user input. Return false if program should close.
bool Simulation::HandleInput(GLFWwindow* window)
{	
	// Not if it is being handled by IMGUI
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.WantCaptureKeyboard || io.WantCaptureMouse)
		return true;

	//Keiths Comments
	{
		// You can get keypresses like this:
		// if (glfwGetKey(window, GLFW_KEY_W)==GLFW_PRESS) // means W key pressed

		// You can get mouse button input, returned state is GLFW_PRESS or GLFW_RELEASE
		// int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

		// Use this to get the mouse position:
		// double xpos, ypos;
		// glfwGetCursorPos(window, &xpos, &ypos);

		// To prevent the mouse leaving the window (and to hide the arrow) you can call:
		// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		// To reenable it use GLFW_CURSOR_NORMAL

		// To see an example of input using GLFW see the camera.cpp file.
	}

	return true;
}


void Simulation::CreateShader(GLuint& programID, const std::string& vertexShaderShaderFile, const std::string& fragmentShaderFile)
{
	programID = glCreateProgram();

	GLuint
		vertex_shader_programID = Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, vertexShaderShaderFile),
		fragment_shader_programID = Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, fragmentShaderFile);

	glAttachShader(programID, vertex_shader_programID);
	glAttachShader(programID, fragment_shader_programID);

	glDeleteShader(vertex_shader_programID);
	glDeleteShader(fragment_shader_programID);

	Helpers::LinkProgramShaders(programID);

}

void Simulation::CreateShaders()
{
	CreateShader(m_program, "Data/Shaders/vertex_shader.vert", "Data/Shaders/fragment_shader.frag");
	CreateShader(m_skyBoxProgram, "Data/Shaders/SkyBox.vert", "Data/Shaders/SkyBox.frag");
	CreateShader(m_cubeProgram, "Data/Shaders/Cube.vert", "Data/Shaders/Cube.frag");
}

// Update the simulation (and render) returns false if program should close
bool Simulation::Update(GLFWwindow* window)
{
	//Handle user input
	if (!HandleInput(window))
		return false;

	//Calculate delte time
	float timeNow = (float)glfwGetTime();
	float deltaTime{ (timeNow - m_lastTime ) };
	m_lastTime = timeNow;

	//Update the camera
	m_camera->Update(window, deltaTime);

	//Set the camera position in the main shader for terrain and models so specular reflection works correctly
	GLuint cameraPositionLocation = glGetUniformLocation(m_program, "camera_position");
	glUniform3f(cameraPositionLocation, m_camera->GetPosition().x, m_camera->GetPosition().y, m_camera->GetPosition().z);

	//Update the models
	for (const std::shared_ptr<MyModel>& model : m_models)
		model->Update(window);

	// Render the scene
	m_renderer->Render(*m_camera, deltaTime, m_models);

	//Create and render the GUI instance for the current frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	m_renderer->DefineGUI(*m_camera);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return true;
}

Simulation::~Simulation()
{
	glDeleteProgram(m_program);
	glDeleteProgram(m_skyBoxProgram);
	glDeleteProgram(m_cubeProgram);
}
