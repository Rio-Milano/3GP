#ifndef TERRAINMODEL_H
#define TERRAINMODEL_H


#include<glm/glm.hpp>
#include<string>

#include"MyModel.h"

class TerrainModel final : public MyModel
{
public:

	TerrainModel(
		const GLuint& programID,
		const std::string& modelName,
		const glm::vec3& position = glm::vec3(0, 0, 0), 
		const glm::vec2& tileSizeArg = glm::vec2(100,100),
		const float& xCellsArg = 100.0f, 
		const float& zCellsArg = 100.0f,
		const std::string& terrainTextureDirArg = "Data\\Textures\\graveld.jpg", 
		const std::string& heightMapDirArg = "Data\\Heightmaps\\curvy.GIF"
	)
		: 
		MyModel(programID, modelName, position), 
		tileSize(tileSizeArg), 
		xCells(xCellsArg), 
		zCells(zCellsArg), 
		terrainTextureDir(terrainTextureDirArg), 
		terrainHeightMapTextureDir(heightMapDirArg){};



private:
	void InitializeModel()override;


	void CreateTerrainPositions(std::vector<glm::vec3>& terrainPositionsArg, const unsigned int& xVertsArg, const unsigned int& zVertzArg, const glm::vec3& startingTerrainPosArg);
	void CreateTerrainElements(std::vector<unsigned int>& elementsArg, const unsigned int& xCellsArg, const unsigned int& zCellsArg);
	void CreateTerrainTexture(std::vector<glm::vec2>& textureCordsArgArg, const unsigned int& xVertsArg, const unsigned int& zVertzArg, GLuint& textureID);
	void ApplyHeightMapToVertexPositions(std::vector<glm::vec3>& terrainPositionsArg, const unsigned int& xVertsArg, const unsigned int& zVertzArg);
	void CalculateTerrainNormals(std::vector<glm::vec3>& normalsArg, const glm::vec3& baseNormalArg, const std::vector<glm::vec3>& terrainPositionsArg, const std::vector<unsigned int>& elementsArg);

	float GetNoiseXY(const int& x, const int& y);
	float CosineLerp(float a, float b, float x);
	float KenPerlin(float xPos, float zPos);


	glm::vec2
		//X = x width, Y = z depth
		tileSize;

		float
			xCells,
			zCells;

	std::string 
		terrainTextureDir,
		terrainHeightMapTextureDir;



};

#endif