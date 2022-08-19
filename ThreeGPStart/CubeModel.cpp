#include "CubeModel.h"
#include"RiosHelpers.h"
#include"Node.h"

CubeModel::CubeModel(const GLuint& program, const std::string& ModelName, const glm::vec3& modelTransform, const glm::vec3& modelRotation, const glm::vec3& modelScale, const bool& rotateCube)
	:MyModel(program, ModelName, modelTransform, modelRotation, modelScale), m_rotateCube(rotateCube)
{
}

void CubeModel::Update(GLFWwindow* window)
{

	if (m_rotateCube)
	{
		if (rotateY) // Rotate around y axis		
			worldMat = glm::rotate(worldMat, glm::radians(1.f), glm::vec3(0, 1, 0));
		else // Rotate around x axis		
			worldMat = glm::rotate(worldMat, glm::radians(1.f), glm::vec3(1, 0, 0));;


		angle += 1.f;
		if (angle >= 720.0f)
		{
			angle = 0;
			rotateY = !rotateY;
		}
	}

}

void CubeModel::InitializeModel()
{

	GLuint positionsBuffer{};
	RiosHelpers::GenBindDataUnbind_Buffer(positionsBuffer, GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	GLuint colorBuffer{};
	RiosHelpers::GenBindDataUnbind_Buffer(colorBuffer, GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

	GLuint elementPositionsBuffer{};
	RiosHelpers::GenBindDataUnbind_Buffer(elementPositionsBuffer, GL_ELEMENT_ARRAY_BUFFER, positionElements.size() * sizeof(unsigned int), positionElements.data(), GL_STATIC_DRAW);

	std::shared_ptr<Node> node{ std::make_shared<Node>() };
	node->m_mesh = std::make_shared<Mesh>();
	node->m_mesh->m_numElements = static_cast<GLuint>(positionElements.size());

	glGenVertexArrays(1, &node->m_mesh->m_VAO);
	glBindVertexArray(node->m_mesh->m_VAO);

	RiosHelpers::EnableBindPointerUnbind_VertexAttribute(0, positionsBuffer, GL_ARRAY_BUFFER, 3 , GL_FLOAT, 3 * sizeof(float));
	RiosHelpers::EnableBindPointerUnbind_VertexAttribute(1, colorBuffer, GL_ARRAY_BUFFER, 3 , GL_FLOAT, 3 * sizeof(float));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementPositionsBuffer);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	m_nodes.emplace_back(node);
	
}

