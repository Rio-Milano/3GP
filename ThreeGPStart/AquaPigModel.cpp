#include "AquaPigModel.h"

#include"Node.h"

AquaPigModel::AquaPigModel(const GLuint& program, const std::string& ModelName, const glm::vec3& modelTransform, const glm::vec3& modelRotation, const glm::vec3& modelScale)
	:MyModel(program, ModelName, modelTransform, modelRotation, modelScale)
{
}

void AquaPigModel::Update(GLFWwindow* window)
{
	//move and roate the whole model
	float modulator{ (float)sin(glfwGetTime()) / 6.0f};
	
	std::shared_ptr<Node> hull{ GetNodeWithNameFromHirearchy(m_rootNode, "hull") };
	hull->worldMatrix = glm::rotate(hull->worldMatrix, glm::radians(modulator/3), glm::vec3(1, 1, 1));
	hull->worldMatrix = glm::translate(hull->worldMatrix, glm::vec3(modulator/20.f, modulator/30.f, 0));

	std::shared_ptr<Node> gunNode{ GetNodeWithNameFromHirearchy(m_rootNode, "gun")};
	gunNode->worldMatrix = glm::rotate(gunNode->worldMatrix, glm::radians(modulator), glm::vec3(1, 0, 0));

	std::shared_ptr<Node> propellerNode{ GetNodeWithNameFromHirearchy(m_rootNode, "propeller")};
	propellerNode->worldMatrix = glm::rotate(propellerNode->worldMatrix, glm::radians(1.1f), glm::vec3(0, 1, 0));
}

void AquaPigModel::InitializeHirearchy()
{
	std::shared_ptr<Node>
		hullNode{ GetNodeWithNameFromMeshVector("hull") },
		wingLeftNode{ GetNodeWithNameFromMeshVector("wing_left") },
		wingRightNode{ GetNodeWithNameFromMeshVector("wing_right") },
		propellerNode{ GetNodeWithNameFromMeshVector("propeller") },
		gunBaseNode{ GetNodeWithNameFromMeshVector("gun_base") },
		gunNode{ GetNodeWithNameFromMeshVector("gun") };

	


	//Configure Nodes
	hullNode->InitNode(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	wingLeftNode->InitNode(glm::vec3(2.231f, 0.272f, -2.663f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	wingRightNode->InitNode(glm::vec3(-2.231f, 0.272f, -2.663), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	propellerNode->InitNode(glm::vec3(0.f, 1.395f, -3.616f), glm::vec3(90, 0, 0), glm::vec3(1, 1, 1));

	gunBaseNode->InitNode(glm::vec3(0.f, 0.599f, -1.866f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	gunNode->InitNode(glm::vec3(0.f, 1.506f, 0.644f), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	//Establish Hirearchy
	m_rootNode = hullNode;

	hullNode->m_children.push_back(wingLeftNode);
	hullNode->m_children.push_back(wingRightNode);
	hullNode->m_children.push_back(gunBaseNode);
	hullNode->m_children.push_back(propellerNode);


	gunBaseNode->m_children.push_back(gunNode);




}
