#include "SkyBoxModel.h"
#include"ImageLoader.h"
#include"Mesh.h"
#include"RiosHelpers.h"
#include"Node.h"

SkyBoxModel::SkyBoxModel(GLuint& programID, const std::string& ModelName)
	:MyModel(programID, ModelName)
{
}

SkyBoxModel::~SkyBoxModel()
{
	glDeleteTextures(1, &m_cubeMapTextureID);
}

void SkyBoxModel::InitializeModel()
{
	//Generate a texture buffer
	glGenTextures(1, &m_cubeMapTextureID);
	//Bind the texture buffer to a cube map
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapTextureID);//bind to cube map texture

	//loop through cube map faces
	for (size_t skyBoxFaceIndex{ 0 }; skyBoxFaceIndex < skyBoxFaces.size(); skyBoxFaceIndex++)
	{
		//make an image loader object to load current sky box face into
		Helpers::ImageLoader currentCubeMapTexture;

		if (!currentCubeMapTexture.Load(m_modelMaterialVector[skyBoxFaceIndex].diffuseTextureFilename))
		{
			std::cout << std::string("Texture: " + m_modelMaterialVector[skyBoxFaceIndex].diffuseTextureFilename + " was not found!\n");
			return;
		}

		//load texture data into texture buffer at current face
		glTexImage2D(skyBoxFaces[skyBoxFaceIndex], 0, GL_RGBA, currentCubeMapTexture.Height(), currentCubeMapTexture.Width(), 0, GL_RGBA, GL_UNSIGNED_BYTE, currentCubeMapTexture.GetData());

		//clear the file name so it is not reloded when buffers are made
		m_modelMaterialVector[skyBoxFaceIndex].diffuseTextureFilename = "";
	}

	//set texture filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//set wrapping s, t, r
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//make minimap texture
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	//we only need position data and element data for the sky box shader

	for (Helpers::Mesh& mesh : m_modelLoader->GetMeshVector())
	{
		std::shared_ptr<Node> node{std::make_shared<Node>()};
		node->m_mesh = std::make_shared<Mesh>();
		
		GLuint vbo;
		RiosHelpers::GenBindDataUnbind_Buffer(vbo, GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3), mesh.vertices.data(), GL_STATIC_DRAW);


		GLuint ebo;
		RiosHelpers::GenBindDataUnbind_Buffer(ebo, GL_ARRAY_BUFFER, mesh.elements.size() * sizeof(unsigned int), mesh.elements.data(), GL_STATIC_DRAW);

		glGenVertexArrays(1, &node->m_mesh->m_VAO);
		glBindVertexArray(node->m_mesh->m_VAO);

		RiosHelpers::EnableBindPointerUnbind_VertexAttribute(0, vbo, GL_ARRAY_BUFFER, 3, GL_FLOAT, sizeof(glm::vec3));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		node->m_mesh->m_numElements = static_cast<GLuint>(mesh.elements.size());

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_nodes.emplace_back(node);
	}

}
void SkyBoxModel::DrawNodeVector(const glm::mat4& worldMatrix)
{

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->m_cubeMapTextureID);
	glUniform1i(glGetUniformLocation(m_program, "skyBox_CubeMapID"), 0);

	for (std::shared_ptr<Node> node : m_nodes)
	{
		glBindVertexArray(node->m_mesh->m_VAO);
		glDrawElements(GL_TRIANGLES, node->m_mesh->m_numElements, GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}

	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);

}
