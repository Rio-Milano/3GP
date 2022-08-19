#include "LightModel.h"

LightModel::LightModel(
	const GLuint& programID,
	const std::string& modelName, 
	const int& lightIndex,
	const glm::vec3& lightColor, 
	const float& lightIntentity,
	const LightType& lightType
)
	
	:
	MyModel(programID, modelName), 
	m_lightType(lightType), 
	m_lightColor(lightColor),
	m_lightIntensity(lightIntentity)

{
}

LightModel::LightModel(
	const GLuint& programID,
	const std::string& modelName, 
	const int& lightIndex,
	const glm::vec3& lightDirection, 
	const glm::vec3& lightColor, 
	const float& lightIntensity,
	const LightType& lightType
)

	:
	MyModel(programID, modelName),
	m_lightType(lightType),
	m_lightIndex(lightIndex),
	m_lightDirection(lightDirection),
	m_lightColor(lightColor),
	m_lightIntensity(lightIntensity)

{
}

LightModel::LightModel(
	const GLuint& programID,
	const std::string& modelName, 
	const int& lightIndex,
	const glm::vec3& lightPosition,
	const glm::vec3& lightScale,
	const glm::vec3& lightColor, 
	const float& lightIntensity, 
	const float& lightRange,
	const LightType& lightType
)

	:
	MyModel(programID, modelName, lightPosition, glm::vec3(0, 0, 0), lightScale),
	m_lightType(lightType),
	m_lightIndex(lightIndex),
	m_lightColor(lightColor),
	m_lightIntensity(lightIntensity),
	m_lightRange(lightRange)

{
}

LightModel::LightModel(
	const GLuint& programID,
	const std::string& modelName, 
	const int& lightIndex,
	const glm::vec3& lightPosition, 
	const glm::vec3& lightScale,
	const glm::vec3& lightDirection, 
	const glm::vec3& lightColor, 
	const float& lightIntensity, 
	const float& lightRange, 
	const float& lightFov,
	const LightType& lightType
)

	:
	MyModel(programID, modelName, lightPosition, glm::vec3(0, 0, 0), lightScale),
	m_lightType(lightType),
	m_lightIndex(lightIndex),
	m_lightDirection(lightDirection),
	m_lightColor(lightColor),
	m_lightIntensity(lightIntensity),
	m_lightRange(lightRange),
	m_lightFov(lightFov)

{
}

void LightModel::Update(GLFWwindow* window)
{
	glUseProgram(m_program);

	const std::string light = std::to_string(m_lightIndex);
	
	if (m_lightType == LightType::Ambient)
	{
		GLuint colorLocation = glGetUniformLocation(m_program, "ambient_light.color");
		GLuint intensityLocation = glGetUniformLocation(m_program, "ambient_light.intensity");

		if (lightOn)
		{
			glUniform3f(colorLocation, m_lightColor.x, m_lightColor.y, m_lightColor.z);
			glUniform1f(intensityLocation, m_lightIntensity);
		}
		else
		{
			glUniform1f(intensityLocation, 0);
		}
	}
	else
	{
		GLuint lightTypeLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].lightType").c_str());
		GLuint positionLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].position").c_str());
		GLuint directionLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].direction").c_str());
		GLuint colorLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].color").c_str());
		GLuint intensityLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].intensity").c_str());
		GLuint rangeLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].range").c_str());
		GLuint fovLocation = glGetUniformLocation(m_program, std::string("lights[" + light + "].fov").c_str());
		if (lightOn)
		{
			glUniform1i(lightTypeLocation, m_lightType);
			glUniform3f(positionLocation, worldMat[3][0], worldMat[3][1], worldMat[3][2]);
			glUniform3f(directionLocation, m_lightDirection.x, m_lightDirection.y, m_lightDirection.z);
			glUniform3f(colorLocation, m_lightColor.x, m_lightColor.y, m_lightColor.z);
			glUniform1f(intensityLocation, m_lightIntensity);
			glUniform1f(rangeLocation, m_lightRange);
			glUniform1f(fovLocation, m_lightFov);
		}
		else
		{
			glUniform1f(intensityLocation, 0);

		}
	}

	float lightMovementSpeed = 1.f;

	glm::vec3 translate = glm::vec3(0, 0, 0);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		translate.z -= lightMovementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		translate.z += lightMovementSpeed;
	}	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		translate.x -= lightMovementSpeed;

	}	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		translate.x += lightMovementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
	{
		translate.y += lightMovementSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
	{
		translate.y -= lightMovementSpeed;
	}

	worldMat = glm::translate(worldMat, translate);


}
