#ifndef RIOSHELPERS_H
#define RIOSHELPERS_H

#include"glew.h"
#include"Mesh.h"
#include"ImageLoader.h"
#include"glm/glm.hpp"
#include"Node.h"

namespace RiosHelpers
{
	inline void GenBindDataUnbind_Buffer(GLuint& bufferID, const GLenum& bufferEnum, const size_t& bytesToCopy, void* data, GLenum drawType)
	{
		glGenBuffers(1, &bufferID);
		glBindBuffer(bufferEnum, bufferID);
		glBufferData(bufferEnum, bytesToCopy, data, drawType);
		glBindBuffer(bufferEnum, 0);
	};

	//must bind to VAO before use
	inline void EnableBindPointerUnbind_VertexAttribute(
		const unsigned int& attributeIndex,
		const unsigned int& bufferID,
		const GLenum& bufferEnum,
		const GLsizei componentSize,
		const GLenum& componentType,
		const GLsizei& componentStride,
		const bool& isNormalized = false,
		size_t componentOffset = 0
		)
	{
		glEnableVertexAttribArray(attributeIndex);
		glBindBuffer(bufferEnum, bufferID);
		glVertexAttribPointer(attributeIndex, componentSize, componentType, isNormalized, componentStride, (void*)componentOffset);
		glBindBuffer(bufferEnum, 0);

	};

	inline bool LoadExternalMeshesIntoModelLoader(Helpers::ModelLoader& theModelMesh , const std::string& dir, const std::vector<std::string>& fileNames, const std::string fileExtension)
	{
		for (const std::string& fileName : fileNames)
		{
			if (!theModelMesh.LoadFromFile(dir + fileName + fileExtension))
				return false;	
			theModelMesh.GetMeshVector().back().name = fileName;
		}
		return true;
	}

	inline void CreateBuffersAndStreamForMesh(Helpers::Mesh& sourceModelMesh, std::shared_ptr<Mesh>& destinationModelMesh, const std::string& textureFileDirNameExt = "")
	{
		/*
		Buffer generation
		*/
		GLuint positions;
		GenBindDataUnbind_Buffer(positions, GL_ARRAY_BUFFER, sourceModelMesh.vertices.size() * sizeof(glm::vec3), sourceModelMesh.vertices.data(), GL_STATIC_DRAW);

		GLuint normals;
		GenBindDataUnbind_Buffer(normals, GL_ARRAY_BUFFER, sourceModelMesh.normals.size() * sizeof(glm::vec3), sourceModelMesh.normals.data(), GL_STATIC_DRAW);
		
		GLuint texCords;
		GenBindDataUnbind_Buffer(texCords, GL_ARRAY_BUFFER, sourceModelMesh.uvCoords.size() * sizeof(glm::vec2), sourceModelMesh.uvCoords.data(), GL_STATIC_DRAW);

		GLuint elements;
		GenBindDataUnbind_Buffer(elements, GL_ELEMENT_ARRAY_BUFFER, sourceModelMesh.elements.size() * sizeof(unsigned int), sourceModelMesh.elements.data(), GL_STATIC_DRAW);
		
		/*
		Texture generation
		*/
		if (textureFileDirNameExt != "")
		{
			Helpers::ImageLoader texture;
			if (!texture.Load(textureFileDirNameExt))
			{
				std::cerr << "ERROR: Failed to load texture at directory:" + textureFileDirNameExt + "\n";
				return;
			}

			glGenTextures(1, &destinationModelMesh->textureID);
			glBindTexture(GL_TEXTURE_2D, destinationModelMesh->textureID);


			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.Width(), texture.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.GetData());

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);
		}

		destinationModelMesh->m_numElements = static_cast<GLuint>(sourceModelMesh.elements.size());

		glGenVertexArrays(1, &destinationModelMesh->m_VAO);//gen vertex array
		glBindVertexArray(destinationModelMesh->m_VAO);//bind vertex array

		EnableBindPointerUnbind_VertexAttribute(0, positions, GL_ARRAY_BUFFER, 3, GL_FLOAT, sizeof(glm::vec3));

		EnableBindPointerUnbind_VertexAttribute(1, normals, GL_ARRAY_BUFFER, 3, GL_FLOAT, sizeof(glm::vec3));
		
		EnableBindPointerUnbind_VertexAttribute(2, texCords, GL_ARRAY_BUFFER, 2, GL_FLOAT, sizeof(glm::vec2));


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);//bind the elements to the VAO

		glBindVertexArray(0);//unbind vertex array
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);//unbind the ebo afer unbind from vao

	}


}

#endif