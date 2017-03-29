#include "Terrain.h"
#include <algorithm>
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <iomanip>


Terrain::Terrain()
{
	this->m_terrainModel = Model("ModelStuff/NanoSluit/TestTerrain.obj", glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));

	readVertices();
	createGrid();
	//sorted in order of x coord -> 
}


Terrain::~Terrain()
{
}

void Terrain::readVertices()
{ //model space
	ifstream file;
	file.open("ModelStuff/NanoSluit/correctVertices.txt", std::fstream::in | std::fstream::out);
	file.seekg(0, file.end);
	int length = file.tellg();
	file.seekg(0, file.beg);

	char * buffer = new char[length];
	file.read(buffer, length);
	file.close();
	int noOfLines = 1;

	for (int i = 0; i < length; i++)
	{
		if (buffer[i] == '\n') noOfLines++;
	}

	int counter = -1;
	for (int k = 0; k < noOfLines; k++)
	{
		glm::vec3 tempXYZ;
		for (int j = 0; j < 3; j++)
		{
			string temp;

			while (counter++ < length)
			{
				if (buffer[counter] == '\n')
				{
					break;
				}
				else if (buffer[counter] == ' ')
				{
					break;
				}
				else
				{
					temp.push_back(buffer[counter]);
				}
			}

			if (j == 0) tempXYZ.x = stof(temp);

			else if (j == 1) tempXYZ.y = stof(temp);

			else tempXYZ.z = stof(temp);

		}
		m_terrainXYZ.push_back(tempXYZ);
	}
}

void Terrain::createGrid()
{
	//correct length value after deleting duplicates and set the number of columns/rows
	int length = sqrt(m_terrainXYZ.size());
	m_gridRows = length;
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
	x += (m_gridRows)/ 2;
	z += (m_gridRows)/ 2;

	//make sure that the position is in the terrain;
	if (!(z > 0 && x > 0 && z < m_grid[m_grid.size() - 1].y &&x < m_grid[m_grid.size() - 1].x))
	{
		std::cout << "Player Outside of Terrain\n";
		return 0;
	}

	//top left corner of a sqaure in the grid
	int gridPos = std::floor(x) + (m_gridRows) * std::floor(z);

	 //convert to square space;
	x -= std::floor(x);
	z -= std::floor(z);

	//use barrycentric interpolation to get height at point x,z. remember m_gridRows = number of vertices - 1.
	if(x < 1-z)
	{
		std::cout << "x\n";
		return barryInterpolation(glm::vec3(0, m_terrainXYZ[gridPos].y, 0), glm::vec3(1, m_terrainXYZ[gridPos + 1].y, 0), glm::vec3(0, m_terrainXYZ[gridPos + m_gridRows].y, 1), glm::vec2(x, z));
	}
	else {
		std::cout << "z\n";

		return barryInterpolation(glm::vec3(1, m_terrainXYZ[gridPos + 1].y, 0),
			glm::vec3(0, m_terrainXYZ[gridPos + m_gridRows].y, 1), glm::vec3(1, m_terrainXYZ[gridPos + m_gridRows + 1].y, 1),glm::vec2(x, z));
	}
}

//might be better to move this to a maths class/file later.
float Terrain::barryInterpolation(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

