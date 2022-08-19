#include "TerrainModel.h"

#include"ImageLoader.h"
#include"RiosHelpers.h"
#include"Node.h"

void TerrainModel::InitializeModel()
{

	std::shared_ptr<Node> node{ std::make_shared<Node>() };
	node->m_mesh = std::make_shared<Mesh>();

	//establish some useful data
	float
		terrainWidth{ tileSize.x * xCells },
		terrainDepth{ tileSize.y * zCells },
		startX{ worldMat[3][0] - (terrainWidth / 2.0f) },
		startY{ worldMat[3][1] },
		startZ{ worldMat[3][2] - (terrainDepth / 2.0f) };

	glm::vec3 startingTerrainPosition
		(worldMat[3][0] - (terrainWidth / 2.0f), worldMat[3][1], worldMat[3][2] - (terrainDepth / 2.0f));

	unsigned int 
		xVerts{(unsigned int)xCells + 1},
		zVerts{(unsigned int)zCells + 1},
		totalVerts{xVerts * zVerts},
		totalCells{(unsigned int)xCells * (unsigned int)zCells};
	//
	// 
	//make the terrain positions
	std::vector<glm::vec3> terrainPositions{};
	terrainPositions.resize(totalVerts);
	CreateTerrainPositions(terrainPositions, xVerts, zVerts, startingTerrainPosition);
	//
	// 
	//make the elements for the ebo
	std::vector<unsigned int> elements{};
	elements.resize((size_t)totalCells * 6);
	CreateTerrainElements(elements, xCells, zCells);
	node->m_mesh->m_numElements= elements.size();
	//
	// 
	//make terrain texture cords
	std::vector<glm::vec2> terrainTextureCords{};
	terrainTextureCords.resize(terrainPositions.size());
	CreateTerrainTexture(terrainTextureCords, xVerts, zVerts, node->m_mesh->textureID);
	//
	//
	//load height map and apply it to terrain positions
	ApplyHeightMapToVertexPositions(terrainPositions, xVerts, zVerts);
	//
	//
	//calculate terrain normals
	glm::vec3 baseNormal{0, startY, 0};
	std::vector<glm::vec3> normals{};
	normals.resize(terrainPositions.size());
	CalculateTerrainNormals(normals, baseNormal, terrainPositions, elements);
	
	//create positions buffer
	GLuint positions{0};
	RiosHelpers::GenBindDataUnbind_Buffer(positions, GL_ARRAY_BUFFER, terrainPositions.size() * sizeof(glm::vec3), terrainPositions.data() ,GL_STATIC_DRAW);

	//create texture cords
	GLuint textureCords{0};
	RiosHelpers::GenBindDataUnbind_Buffer(textureCords, GL_ARRAY_BUFFER, terrainTextureCords.size() * sizeof(glm::vec2), terrainTextureCords.data(), GL_STATIC_DRAW);

	GLuint normalBuffer{0};
	RiosHelpers::GenBindDataUnbind_Buffer(normalBuffer, GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	GLuint elementsBuffer{ 0 };
	RiosHelpers::GenBindDataUnbind_Buffer(elementsBuffer, GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

	//gen VAO
	glGenVertexArrays(1, &node->m_mesh->m_VAO);
	glBindVertexArray(node->m_mesh->m_VAO);
	
	RiosHelpers::EnableBindPointerUnbind_VertexAttribute(0, positions, GL_ARRAY_BUFFER, 3, GL_FLOAT, sizeof(glm::vec3));
	RiosHelpers::EnableBindPointerUnbind_VertexAttribute(1, normalBuffer, GL_ARRAY_BUFFER, 3, GL_FLOAT, sizeof(glm::vec3));
	RiosHelpers::EnableBindPointerUnbind_VertexAttribute(2, textureCords, GL_ARRAY_BUFFER, 2, GL_FLOAT, sizeof(glm::vec2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	m_nodes.emplace_back(node);
}


void TerrainModel::CreateTerrainPositions(std::vector<glm::vec3>& terrainPositionsArg, const unsigned int& xVertsArg, const unsigned int& zVertzArg, const glm::vec3& startingTerrainPosArg)
{
	//loop depth
	for (unsigned int vertexZ{ 0 }; vertexZ < zVertzArg; ++vertexZ)
	{
		//loop width
		for (unsigned int vertexX{ 0 }; vertexX < xVertsArg; ++vertexX)
			terrainPositionsArg[(size_t)vertexX + vertexZ * zVertzArg] = glm::vec3(startingTerrainPosArg.x + vertexX * tileSize.x, startingTerrainPosArg.y, startingTerrainPosArg.z + vertexZ * tileSize.y);
	}
}

void TerrainModel::CreateTerrainElements(std::vector<unsigned int>& elementsArg, const unsigned int& xCellsArg, const unsigned int& zCellsArg)
{
	//use diamond tessellation by flipping between the way in which elements are ordered 
	bool tickTriangle{ false };

	unsigned int xVertsAccross{xCellsArg+1};

	//loop depth
	for (unsigned int cellZ{ 0 }; cellZ < zCells; ++cellZ)
	{
		//loop width
		for (unsigned int cellX{ 0 }; cellX < xCells; ++cellX)
		{
			unsigned int currentVertexIndex{ cellX + cellZ * xVertsAccross };

			if (!tickTriangle)
			{
				//first triangle

				elementsArg.emplace_back(currentVertexIndex);
				elementsArg.emplace_back(currentVertexIndex + xVertsAccross);
				elementsArg.emplace_back(currentVertexIndex + 1);

				//seccond triangle

				elementsArg.emplace_back(currentVertexIndex + 1);
				elementsArg.emplace_back(currentVertexIndex + xVertsAccross);
				elementsArg.emplace_back(currentVertexIndex + xVertsAccross + 1);
			}
			else
			{
				//first triangle

				elementsArg.emplace_back(currentVertexIndex);
				elementsArg.emplace_back(currentVertexIndex + xVertsAccross);
				elementsArg.emplace_back(currentVertexIndex + xVertsAccross + 1);

				//seccond triangle

				elementsArg.emplace_back(currentVertexIndex);
				elementsArg.emplace_back(currentVertexIndex + xVertsAccross + 1);
				elementsArg.emplace_back(currentVertexIndex + 1);

			}

			//after each width pass switch method
			tickTriangle = !tickTriangle;
		}
		//after each depth pass switch method
		tickTriangle = !tickTriangle;
	}
}

void TerrainModel::CreateTerrainTexture(std::vector<glm::vec2>& textureCordsArgArg, const unsigned int& xVertsArg, const unsigned int& zVertzArg, GLuint& textureID)
{
	Helpers::ImageLoader textureTiledImage;
	if (!textureTiledImage.Load(this->terrainTextureDir))
	{
		std::cout << "Terrain Image failed to load\n";
		return;
	}

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	////remove the red and green channels
	//for (int x = 0; x < textureTiledImage.Width(); x++)
	//{
	//	for (int y = 0; y < textureTiledImage.Height(); y++)
	//	{
	//		BYTE* textureTexel{ textureTiledImage.GetData() + (x + y * textureTiledImage.Width()) * 4 };
	//		textureTexel[2] = textureTexel[0] % textureTexel[1] * textureTexel[0] % textureTexel[1];
	//		textureTexel[2] -= rand() > .1 ? textureTexel[2] / 2 : 0;
	//		textureTexel[0] = 0;
	//		textureTexel[1] = 0;
	//		textureTexel[3] = 255;
	//	}
	//}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureTiledImage.Width(), textureTiledImage.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, textureTiledImage.GetData());

	//wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	//unbind texture buff
	glBindTexture(GL_TEXTURE_2D, 0);


	
	for (int vertexZ{ 0 }; vertexZ < zVertzArg; vertexZ++)
	{
		for (int vertexX{ 0 }; vertexX < xVertsArg; vertexX++)
			textureCordsArgArg[vertexX + vertexZ * xVertsArg] = glm::vec2(vertexX, -vertexZ/*startUV.x + vertexX / xCells, startUV.y - vertexZ / zCells*/);
	}
}

void TerrainModel::ApplyHeightMapToVertexPositions(std::vector<glm::vec3>& terrainPositionsArg, const unsigned int& xVertsArg, const unsigned int& zVertzArg)
{
	Helpers::ImageLoader heightMap;
	if (!heightMap.Load(this->terrainHeightMapTextureDir))
	{
		std::cout << "Height map failed to load!\n";
		return;
	}

	float
		//increase the amplitude of the terrain by some amount
		heightMapFieldAmplitudeCoeficcient{ 5 },
		noiseAmplitiudeCoefficient{3},
		//find a ratio for how much texture to cross per vertex on x and y
		xTexelsAccrossForX{ (float)heightMap.Width() / (float)xVertsArg },
		yTexelsAccrossForZ{ (float)heightMap.Height() / (float)zVertzArg };

	//loop depth
	for (int vertexZ{ 0 }; vertexZ < zVertzArg; vertexZ++)
	{
		//loop width
		for (int vertexX{ 0 }; vertexX < xVertsArg; vertexX++)
		{
			//calculate the pixel position
			int
				xPixel{ (int)(xTexelsAccrossForX * vertexX) },
				yPixel{ (int)(yTexelsAccrossForZ * vertexZ) };

			
			size_t pixelOffset{ ((size_t)xPixel + yPixel * heightMap.Width()) * 4 };
			BYTE red = *(heightMap.GetData() + pixelOffset);

			float noiseValue{ KenPerlin(vertexX*tileSize.x, vertexZ*tileSize.y) /*/2.f + 0.5f*/ };
			terrainPositionsArg[vertexX + (vertexZ * zVertzArg)].y += red * heightMapFieldAmplitudeCoeficcient + noiseValue * noiseAmplitiudeCoefficient;
		}
	}

}

void TerrainModel::CalculateTerrainNormals(std::vector<glm::vec3>& normalsArg, const glm::vec3& baseNormalArg, const std::vector<glm::vec3>& terrainPositionsArg, const std::vector<unsigned int>& elementsArg)
{
	//loop through each triangle
	for (int triangle{ 0 }; triangle < elementsArg.size(); triangle += 3)
	{
		glm::vec3
			vertexA{ terrainPositionsArg[elementsArg[triangle + 0]] },
			vertexB{ terrainPositionsArg[elementsArg[triangle + 1]] },
			vertexC{ terrainPositionsArg[elementsArg[triangle + 2]] },

			vectorAB{ vertexB - vertexA },
			vectorAC{ vertexC - vertexA };

		//calculate the normal for that triangle
		glm::vec3 normal{ glm::cross(vectorAB, vectorAC) };

		//add the normal to vertices 
		normalsArg[elementsArg[triangle + 0]] += normal;
		normalsArg[elementsArg[triangle + 1]] += normal;
		normalsArg[elementsArg[triangle + 2]] += normal;

	};

	//re-normalize normals
	for (glm::vec3& normal : normalsArg)
		normal = glm::normalize(normal);
}


float TerrainModel::GetNoiseXY(const int& x, const int& y)
{
	int n = x + y * 57; // 57 is the seed – can be tweaked
	n = (n >> 13) ^ n;
	int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0f - ((float)nn / 1073741824.0f);
}

float TerrainModel::CosineLerp(float a, float b, float x)
{
	float ft = x * 3.1415927f;
	float f = (1.0f - cos(ft)) * 0.5f;
	return a * (1.0f - f) + b * f;
}

float TerrainModel::KenPerlin(float xPos, float zPos)
{
	float s = GetNoiseXY((int)xPos, (int)zPos);
	float t = GetNoiseXY((int)xPos + 1, (int)zPos);
	float u = GetNoiseXY((int)xPos, (int)zPos + 1);
	float v = GetNoiseXY((int)xPos + 1, (int)zPos + 1);
	float c1 = CosineLerp(s, t, xPos);
	float c2 = CosineLerp(u, v, xPos);
	return CosineLerp(c1, c2, zPos);
}
