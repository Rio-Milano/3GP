#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <glew.h>

#include<vector>

namespace COMPONENT
{
	struct Mesh
	{
		GLuint m_VAO{};
		GLuint m_numElements{};
		GLuint textureID{};
	};

	struct Model
	{
		std::vector<Mesh> m_meshes{};
	};

};

#endif