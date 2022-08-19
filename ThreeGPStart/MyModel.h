#ifndef MODEL_H
#define MODEL_H

#include<unordered_map>
#include<glm/glm.hpp>
#include<string>
#include<vector>
#include<memory>
#include<glew.h>

#include"Mesh.h"

struct Node;

namespace Helpers { class ModelLoader; };

class MyModel
{
	friend class Renderer;
public:
	explicit MyModel(const GLuint& program, const std::string& ModelName, const glm::vec3& modelTransform = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& modelRotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& modelScale = glm::vec3(1.0f, 1.0f, 1.0f));
	virtual ~MyModel();

	const std::string& GetModelName()const { return m_modelName; };

	void LoadModelData(const std::string& fileDir, const std::string& modelFileName, const std::string& modelFileExtension, const std::string& textureFileName, const std::string& textureFileExtension);

	void InstansiateModel();

	/*
	General functions
	*/
	virtual void Update(GLFWwindow* window) {};
	virtual void Render(const glm::mat4& combined_projView);


private:
	/*
	sets up buffers, loads textures, sets up vao and streams. Then puts data into mesh object;
	*/
	virtual void InitializeModel();

	/*
	Picks out nodes and makes hirearchy
	*/
	virtual void InitializeHirearchy() {};


	void DrawHirearchy(std::shared_ptr<Node> node = nullptr, glm::mat4 worldMatrix = glm::mat4(1));
	
	virtual void DrawNodeVector(const glm::mat4& worldMatrix);




	/*
	Hirearchy
	*/
	std::unordered_map<std::shared_ptr<Node>, bool> m_visitedNodesMap;

	std::string m_modelName;


protected:
	std::shared_ptr<Node> GetNodeWithNameFromMeshVector(const std::string& nodeName)const;
	std::shared_ptr<Node> GetNodeWithNameFromHirearchy(const std::shared_ptr<Node>& node, const std::string& nodeName)const;

	/*
	Once the needed model data has been loaded and 
	used appropriately this will be deleted
	*/
	Helpers::ModelLoader* m_modelLoader{nullptr};

	//a copy of the matrerial vector so texture file names can be mutated before use
	std::vector<Helpers::Material> m_modelMaterialVector;

	/*
	When a hirearchy has not been established we will render by looping
	- when creating a hirearchy this will be cleared as it is not needed
	*/
	std::vector<std::shared_ptr<Node>> m_nodes;

	GLuint m_program;

	/*
	When a hirearchy has been established the root node will be used.
	The vector of nodes will be picked for nodes to create the hirearchy
	*/
	std::shared_ptr<Node> m_rootNode;

	glm::mat4 worldMat;

	
	
	int m_shininess = 64;
};

#endif
