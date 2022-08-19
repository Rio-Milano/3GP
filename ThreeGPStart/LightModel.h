#ifndef LIGHTMODEL_H
#define LIGHTMODEL_H

#include"MyModel.h"

enum LightType
{
	Ambient = 0,
	Directional = 1,
	PointLight = 2,
	SpotLight = 3

};

class LightModel final : public MyModel
{
public:
	/*
	Ambient light constructor
	*/
	explicit LightModel(
		const GLuint& programID,
		const std::string& modelName,
		const int& lightIndex,
		const glm::vec3& lightColor,
		const float& lightIntentity,
		const LightType& lightType = LightType::Ambient
	);

	/*
	Directional Light
	*/
	explicit LightModel(
		const GLuint& programID,
		const std::string& modelName,
		const int& lightIndex,
		const glm::vec3& lightDirection,
		const glm::vec3& lightColor,
		const float& lightIntensity,
		const LightType& lightType = LightType::Directional
	);

	/*
	Point Light
	*/
	explicit LightModel(
		const GLuint& programID,
		const std::string& modelName,
		const int& lightIndex,
		const glm::vec3& lightPosition,
		const glm::vec3& lightScale,
		const glm::vec3& lightColor,
		const float& lightIntensity,
		const float& lightRange,
		const LightType& lightType = LightType::PointLight
	);

	//Spot Light
	explicit LightModel(
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
		const LightType& lightType = LightType::SpotLight
	);

	void Update(GLFWwindow* window) override;

	void ToggleLight(const bool& lightToggle) { lightOn = lightToggle; };

private:
	LightType m_lightType;
	int m_lightIndex;

	//Use model transform as light position
	glm::vec3 
		m_lightDirection,
		m_lightColor;
	float 
		m_lightIntensity,
		m_lightRange,
		m_lightFov;

	bool lightOn = true;

};

#endif