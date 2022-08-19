#ifndef SKYBOXMODEL_H
#define SKYBOXMODEL_H

#include"MyModel.h"

class SkyBoxModel final : public MyModel
{
public:
	SkyBoxModel(GLuint& programID, const std::string& ModelName);
	~SkyBoxModel();


	void InitializeModel() override;

private:

	void DrawNodeVector(const glm::mat4& worldMatrix);
	//the face types for the cube map (just enums for 0->5)
	std::vector<unsigned int> skyBoxFaces
	{
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y
	};


	//Not using nodes as sky box is too unique compared to general models
	GLuint m_cubeMapTextureID{ 0 };



};

#endif