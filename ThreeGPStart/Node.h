#ifndef NODE_H
#define NODE_H

#include"glm/glm.hpp"


struct Mesh
{
	GLuint m_VAO{};
	GLuint m_numElements{};
	GLuint textureID{};
};

struct Node
{
	void InitNode(const glm::vec3 transform, const glm::vec3 rotation, const glm::vec3 scale)
	{
		worldMatrix = glm::mat4(1);
		
		//apply the current nodes transform to the model matix
		worldMatrix = glm::translate(worldMatrix, transform);

		//apply each rotation to the model matrix
		if (rotation.x)
			worldMatrix = glm::rotate(worldMatrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		if (rotation.y)
			worldMatrix = glm::rotate(worldMatrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		if (rotation.z)
			worldMatrix = glm::rotate(worldMatrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

		worldMatrix = glm::scale(worldMatrix, scale);
	}

	std::string m_nodeName{""};

	std::vector<std::shared_ptr<Node>> m_children{ };
	
	std::shared_ptr<Mesh> m_mesh{nullptr};

	glm::mat4 worldMatrix;



};

#endif
