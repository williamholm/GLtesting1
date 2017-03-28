#include "Terrain.h"
#include <algorithm>
#include <math.h>
#include <set>
bool swapXYZ1(glm::vec3 i, glm::vec3 j)
{
	return (i.z < j.z);
}
bool swapXYZ2(glm::vec3 i, glm::vec3 j)
{
	return (j.z == i.z && i.x< j.x);
}

Terrain::Terrain()
{
	this->m_terrainModel = Model("ModelStuff/NanoSluit/T3.obj", glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	std::vector<Mesh> tempMeshes = m_terrainModel.getMeshes();
	for (int i = 0, z = tempMeshes.size(); i < z; i++)
	{
		for (int j = 0; j < tempMeshes[z-i-1].m_verticesC.size(); j++)
		{
			m_terrainXYZ.push_back(tempMeshes[z-i-1].m_verticesC[j].Position);
		}
		tempMeshes[z-i-1].~Mesh();
		tempMeshes.pop_back();
	}
	sortVertices();
	createGrid();
	//sorted in order of x coord -> 
}


Terrain::~Terrain()
{
}

void Terrain::sortVertices()
{ //model space
  //works only for sqaures.
	int length = sqrt(m_terrainXYZ.size());
	//make sure that it is sqaure
	if (length != sqrt(m_terrainXYZ.size()))
	{
		return;
	}

	//sort so terrain vertices are in order of z value
	std::sort(m_terrainXYZ.begin(), m_terrainXYZ.end(), swapXYZ1);


	//now sort so terrain vertices are in order of x, then z value
	//first row sorted
	sort(m_terrainXYZ.begin(), m_terrainXYZ.begin() + length, swapXYZ2);

	//middle rows sorted
	for (int i = length, j = m_terrainXYZ.size(); i < j - length; i = i + 2 * length)
	{
		sort(m_terrainXYZ.begin() + i, m_terrainXYZ.begin() + i + 2 * length, swapXYZ2);
	}

	//final row sorted
	sort(m_terrainXYZ.end() - length, m_terrainXYZ.end(), swapXYZ2);

	for (int i = 0, j = m_terrainXYZ.size(); i < j; i++)
	{
		if (!(i%length)) std::cout << "\n\n";
		std::cout << "(" << m_terrainXYZ[i].x << "," << m_terrainXYZ[i].z << ")";
	}

	//erase the repeated vertices.
	m_terrainXYZ.erase(unique(m_terrainXYZ.begin(), m_terrainXYZ.end()), m_terrainXYZ.end());
	std::cout << m_terrainXYZ.size();
}

void Terrain::createGrid()
{
	//correct length value after deleting duplicates and set the number of columns/rows
	int length = sqrt(m_terrainXYZ.size());
	m_gridRows = length - 1;
	int temp = m_gridRows / 2;
	m_verticeDistance = m_terrainXYZ[1].x - m_terrainXYZ[0].x;

	for (int i = 0; i < length; i++)
	{
		for (int j = 0; j < length; j++)
		{
			m_grid.emplace_back(glm::ivec2(j,i));
		}
	}
	
}

float Terrain::getHeight(float x, float z)
{
	//convert to grid space;
	x /= m_verticeDistance;
	z /= m_verticeDistance;
	x += m_gridRows / 2;
	z += m_gridRows / 2;

	//make sure that the position is in the terrain;
	if (!(z > 0 && x > 0 && z < m_grid[m_grid.size() - 1].y &&x < m_grid[m_grid.size() - 1].x))
	{
		std::cout << "Player Outside of Terrain\n";
		return 0;
	}

	int gridPos = std::floor(x) + (m_gridRows + 1) * std::floor(z);

	 //convert to square space;
	x -= std::floor(x);
	z -= std::floor(z);
	return m_terrainXYZ[gridPos].y;
//	if(x < 1-z)
//	{
//		std::cout << "x\n";
//		return barryInterpolation(glm::vec3(0, m_terrainXYZ[gridPos].y, 0), glm::vec3(1, m_terrainXYZ[gridPos + 1].y, 0), glm::vec3(0, m_terrainXYZ[gridPos + m_gridRows].y, 1), glm::vec2(x, z));
//	}
//	else {
//		std::cout << "z\n";
//
//		return barryInterpolation(glm::vec3(1, m_terrainXYZ[gridPos + m_gridRows + 1].y, 1), glm::vec3(1, m_terrainXYZ[gridPos + 1].y, 0),
//			glm::vec3(0, m_terrainXYZ[gridPos + m_gridRows].y, 1), glm::vec2(x, z));
//	}
}

float Terrain::barryInterpolation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

