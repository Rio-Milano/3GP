#ifndef AQUAPIGMODEL_H
#define AQUAPIGMODEL_H

#include"MyModel.h"

class AquaPigModel final : public MyModel
{
public:
	explicit AquaPigModel(const GLuint& program, const std::string& ModelName, const glm::vec3& modelTransform = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& modelRotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& modelScale = glm::vec3(1.0f, 1.0f, 1.0f));
	
	void Update(GLFWwindow* window) override;

private:
	void InitializeHirearchy() override;
	
};

#endif