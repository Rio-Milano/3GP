#ifndef CUBE_H
#define CUBE_H

#include"MyModel.h"


class CubeModel final : public MyModel
{
public:
	CubeModel(const GLuint& program, const std::string& ModelName, const glm::vec3& modelTransform = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& modelRotation = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& modelScale = glm::vec3(1.0f, 1.0f, 1.0f), const bool& rotateCube = true);
	~CubeModel() = default;

	void Update(GLFWwindow* window) override;


private:

	void InitializeModel() override;

	bool m_rotateCube;
	float angle = 0.0f;
	bool rotateY = false;

	std::vector<float> positions
	{
		//TOP FACE
		//top left corner
			-10.0f, 10.0f, -10.0f,//0
		//top right corner
			10.0f, 10.0f, -10.0f,//1
		//bottom left corner
			-10.0f, 10.0f, 10.0f,//2
		//bottom right corner
			10.0f, 10.0f, 10.0f,//3

		//BOTTOM FACE
		//top left corner
			-10.0f, -10.0f, -10.0f,//4
		//top right corner
			10.0f, -10.0f, -10.0f,//5
		//bottom left corner
			-10.0f, -10.0f, 10.0f,//6
		//bottom right corner
			10.0f, -10.0f, 10.0f,//7

		//FRONT FACE
		//top left corner
			-10.0f, 10.0f, 10.0f,//8
		//top right corner
			10.0f, 10.0f, 10.0f,//9
		//bottom left corner
			-10.0f, -10.0f, 10.0f,//10
		//bottom right corner
			10.0f, -10.0f, 10.0f,//11

		//RIGHT FACE
		//top right corner
			10.0f, 10.0f, -10.0f,//12
		//top left corner
			10.0f, 10.0f, 10.0f,//13
		//bottom left corner
			10.0f, -10.0f, 10.0f,//14
		//bottom right corner
			10.0f, -10.0f, -10.0f,//15

		//LEFT FACE
		//top left corner
		-10.0f, 10.0f, -10.0f,//16
		//top right corner
		-10.0f, 10.0f, 10.0f,//17
		//bottom left corner
		-10.0f, -10.0f, -10.0f,//18
		//bottom right corner
		-10.0f, -10.0f, 10.0f,//19

		//BACK FACE
		//top left corner
		-10.0f, 10.0f, -10.0f,//20
		//top right corner
		10.0f, 10.0f, -10.0f,//21
		//bottom left corner
		-10.0f, -10.0f, -10.0f,//22
		//bottom right corner
		10.0f, -10.0f, -10.0f//23
	};

	std::vector<float> colors
	{
		//top face color : RED
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,


		//bottom face color : GREEN
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,


		//front face color : BLUE
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,

		//right face color : WHITE
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		//left face color : PURPLE
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,


		//back face color : YELLOW
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
	};

	std::vector<unsigned int> positionElements
	{
		//TOP FACE
		//top face left triangle
		2, 1, 0,
		//top face right triangle
		2, 3, 1,


		//BOTTOM FACE
		//bottom face left triangle
		4, 5, 6,
		//bottom face right triangle
		5, 7, 6,


		//FRONT FACE
		//right face left triangle
		10, 9, 8,
		//right face right triangle
		10, 11, 9,

		//RIGHT FACE
		//left face left triangle
		14, 12, 13,
		//right face right triangle
		14, 15, 12,

		//LEFT FACE
		//left face left triangle
		18, 17, 16,
		//right face right triangle
		18, 19, 17,

		//BACK FACE
		//back face left triangle
		20, 21, 22,
		//back face right triangle
		21, 23, 22

	};
};

#endif