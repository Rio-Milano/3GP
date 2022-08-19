#include "MyModel.h"

#include"Mesh.h"
#include"Node.h"
#include"RiosHelpers.h"
#include"Helper.h"

MyModel::MyModel(const GLuint& program, const std::string& ModelName, const glm::vec3& modelTransform, const glm::vec3& modelRotation, const glm::vec3& modelScale)
	: m_program(program), m_modelName(ModelName)
{
	m_modelLoader = new Helpers::ModelLoader;

	worldMat = glm::mat4(1);

	worldMat = glm::translate(worldMat, modelTransform);

	if (modelRotation.x)
		worldMat = glm::rotate(worldMat, glm::radians(modelRotation.x), glm::vec3(1, 0, 0));
	if (modelRotation.y)
		worldMat = glm::rotate(worldMat, glm::radians(modelRotation.y), glm::vec3(0, 1, 0));
	if (modelRotation.z)
		worldMat = glm::rotate(worldMat, glm::radians(modelRotation.z), glm::vec3(0, 0, 1));

	worldMat = glm::scale(worldMat, modelScale);

}

MyModel::~MyModel()
{
	//only called if model not finalized
	delete m_modelLoader;
}

void MyModel::LoadModelData(const std::string& fileDir, const std::string& modelFileName, const std::string& modelFileExtension, const std::string& textureFileName, const std::string& textureFileExtension)
{
	//load model file
	if (!m_modelLoader->LoadFromFile(fileDir + modelFileName + modelFileExtension))
	{
		std::cerr << std::string(fileDir + modelFileName + modelFileExtension + " : failed to load");
		return;
	}
	
	//as model files can contain multiple meshes this is a slow but clean way to keep local material vector up to date
	m_modelMaterialVector = m_modelLoader->GetMaterialVector();
	
	for (int i = 0; i < m_modelLoader->GetMaterialVector().size(); i++)
	{
		if(textureFileName == "")
			m_modelMaterialVector[i].diffuseTextureFilename = fileDir + m_modelMaterialVector[i].diffuseTextureFilename;
		else
			m_modelMaterialVector[i].diffuseTextureFilename = fileDir + textureFileName + textureFileExtension;
	}

	m_modelLoader->GetMeshVector()[m_modelLoader->GetMeshVector().size() - 1].name = modelFileName;

}

void MyModel::InstansiateModel()
{
	InitializeModel();
	InitializeHirearchy();
}



void MyModel::InitializeModel()
{
	if (m_modelLoader == nullptr)
		return;

	//Get the vector of meshes for the model
	std::vector<Helpers::Mesh>& meshVector{ m_modelLoader->GetMeshVector() };
	//loop the vector of meshes for the model
	for ( Helpers::Mesh& mesh : meshVector)
	{
		std::shared_ptr<Node> arbitraryNode{std::make_shared<Node>()};
		arbitraryNode->m_mesh = std::make_shared<Mesh>();
		RiosHelpers::CreateBuffersAndStreamForMesh(mesh, arbitraryNode->m_mesh, m_modelMaterialVector[mesh.materialIndex].diffuseTextureFilename);
		
		arbitraryNode->m_nodeName = mesh.name;

		m_nodes.emplace_back(arbitraryNode);
	};

	//delete m_modelLoader;
}
void MyModel::Render(const glm::mat4& combined_projView)
{
	glUseProgram(m_program);

	glUniformMatrix4fv(glGetUniformLocation(m_program, "combined_xform"), 1, GL_FALSE, glm::value_ptr(combined_projView));

	glUniform1i(glGetUniformLocation(m_program, "drawingSelection"), 1 );

	glUniform1i(glGetUniformLocation(m_program, "material.shininess"), m_shininess);
	glUniform1i(glGetUniformLocation(m_program, "material.hasAmbientTextures"), 0);
	glUniform1i(glGetUniformLocation(m_program, "material.hasSpecularTexture"), 0);
	



	if (m_rootNode == nullptr)
		DrawNodeVector(worldMat);
	else
	{
		if (m_nodes.size() > 0)
			m_nodes.clear();


		m_visitedNodesMap.clear();
		DrawHirearchy(m_rootNode, worldMat);
	}
}


void MyModel::DrawHirearchy(std::shared_ptr<Node> node, glm::mat4 worldMatrix)
{
	if (m_rootNode == nullptr)
		return;

	m_visitedNodesMap[node] = true;

	worldMatrix *= node->worldMatrix;

	//Use mesh texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, node->m_mesh->textureID);

	//Set texture ID in shader
	glUniform1i(glGetUniformLocation(m_program, "material.diffuseTexture"), 0);

	//use the VAO of the current mesh
	glBindVertexArray(node->m_mesh->m_VAO);

	//send the model matrix to the shader
	glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(worldMatrix));

	//draw the mesh
	glDrawElements(GL_TRIANGLES, node->m_mesh->m_numElements, GL_UNSIGNED_INT, (void*)0);

	//unbind from the vao
	glBindVertexArray(0);

	//loop through all child nodes
	for (std::shared_ptr<Node> childNode : node->m_children)
	{
		//if child node not visited
		if (!m_visitedNodesMap[childNode])
			DrawHirearchy(childNode, worldMatrix);
	}

	
}

void MyModel::DrawNodeVector (const glm::mat4& worldMat)
{
	for (const std::shared_ptr<Node>& node : m_nodes)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, node->m_mesh->textureID);
		glUniform1i(glGetUniformLocation(m_program, "material.diffuseTexture"), 0);

		glBindVertexArray(node->m_mesh->m_VAO);
		

		
		glUniformMatrix4fv(glGetUniformLocation(m_program, "model_xform"), 1, GL_FALSE, glm::value_ptr(worldMat));

		glDrawElements(GL_TRIANGLES, node->m_mesh->m_numElements, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
}
std::shared_ptr<Node> MyModel::GetNodeWithNameFromMeshVector(const std::string& nodeName) const
{
	for (const std::shared_ptr<Node>& node : m_nodes)
	{
		if(node->m_nodeName == nodeName)
			return node;
	};
	return nullptr;
}
std::shared_ptr<Node> MyModel::GetNodeWithNameFromHirearchy(const std::shared_ptr<Node>& node, const std::string& nodeName) const
{
	if (node->m_nodeName == nodeName)
		return node;

	for (const std::shared_ptr<Node>& node : node->m_children)
	{
		std::shared_ptr<Node> returnedNode{ GetNodeWithNameFromHirearchy(node, nodeName) };
		if(returnedNode != nullptr)
			return returnedNode;
	}
	return nullptr;
};


