#pragma once
#include "Model.h"
class Terrain
{
public:
	Terrain();
	~Terrain();
	Model m_terrainModel;
	vector<glm::vec3> m_terrainXYZ;
	vector<glm::ivec2> m_grid;
	int m_gridRows;
	float m_verticeDistance;
	float getHeight(float x, float z);
	float barryInterpolation(glm::vec3 v, glm::vec3 u, glm::vec3 w, glm::vec2 pos);
private:
	void createGrid();
	void sortVertices();
};

